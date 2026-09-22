"""Exercise the shipped magic loader's nested parsing and Windows CRT token rules.

W3DMagic.cpp ReadEffect does NOT clear szKeyword before sscanf. A blank line
after ReadPart consequently calls ReadPart again and consumes the next block.
This deliberately preserves that legacy behavior instead of validating balanced
tags only. Resource rendering is covered separately by verify_skill140_assets.
"""
from pathlib import Path
import ctypes
import json
import re

R = Path(__file__).resolve().parents[1]
D = R / 'client-overlay'
C = R / 'runtime/client/GameClient'
crt = ctypes.CDLL('msvcrt')
crt.sscanf.restype = ctypes.c_int


def parse(raw):
    # fgets(fp, 256, ...) in text mode reads at most 255 bytes per invocation.
    lines = []
    for line in raw.replace(b'\r\n', b'\n').splitlines(keepends=True):
        lines.extend(line[i:i+255] for i in range(0, len(line), 255))
    at = 0
    counts = dict(effects=0, parts=0, blows=0)

    def block(kind, end):
        nonlocal at
        keyword = ctypes.create_string_buffer(80)
        while at < len(lines):
            line = lines[at]; at += 1
            if kind != 'effect':
                keyword.value = b''
            crt.sscanf(line, b'%79s', keyword)
            token = keyword.value
            if token == end:
                return
            if kind == 'magic' and token == b'[EFFECT]':
                counts['effects'] += 1
                block('effect', b'[/EFFECT]')
            elif kind == 'effect' and token == b'[PART]':
                counts['parts'] += 1
                block('part', b'[/PART]')
            elif kind == 'part' and token == b'[BLOWTIMING]':
                counts['blows'] += 1
                block('blow', b'[/BLOWTIMING]')
        raise ValueError(f'{kind} reached EOF before {end.decode()}')

    assert lines and lines[0].split()[0] == b'[NAME]'
    at = 1
    block('magic', b'[/NAME]')
    return counts


def main():
    source = (R/'src/client/engine/Src/W3DMagic.cpp').read_bytes().decode('cp949')
    effect = source.split('EFFECT* CW3DMagicMgr::ReadEffect')[1].split('PART* CW3DMagicMgr::ReadPart')[0]
    assert '*szKeyword = NULL' not in effect
    assert 'sscanf( szBuffer, "%s", szKeyword );' in effect
    # Minimal reproduction: both files have perfectly balanced tags.
    fixture = b'[NAME] regression\n[EFFECT]\n[PART]\n[TIMELIMIT] 5\n[/PART]\n[/EFFECT]\n[/NAME]\n'
    assert parse(fixture) == dict(effects=1, parts=1, blows=0)
    try:
        parse(fixture.replace(b'[/PART]\n', b'[/PART]\n\n'))
    except ValueError as error:
        assert 'part reached EOF' in str(error)
    else:
        raise AssertionError('legacy blank-line regression was not reproduced')

    listing = (D/'Magic/MagicList.ml').read_bytes().decode('cp949')
    results = {}; failures = {}
    for key, filename in re.findall(r'^\[MAGIC\]\s+(\S+)\s+(\S+)', listing, re.M):
        if not key.startswith('mt_'):
            continue
        p = D/'Magic'/filename
        if not p.exists(): p = C/'Magic'/filename
        try:
            result = parse(p.read_bytes())
            assert result['effects'] == p.read_bytes().count(b'[EFFECT]')
            assert result['parts'] == p.read_bytes().count(b'[PART]')
            assert result['blows'] == p.read_bytes().count(b'[BLOWTIMING]')
            results[key] = result
        except (ValueError, AssertionError) as error:
            failures[key] = str(error)
    report = dict(passed=not failures, windows_crt=True, legacy_blank_line_reproduced=True,
                  scripts=results, failures=failures, in_game_visual_test=False)
    (R/'assets/skills140/native-loader-validation.json').write_text(json.dumps(report, indent=2)+'\n')
    print(json.dumps(report, indent=2))
    assert not failures, failures


if __name__ == '__main__': main()

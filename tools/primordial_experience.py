"""Patch only the six primordial rows' EXP column, preserving all other bytes."""
from pathlib import Path
import argparse
import hashlib
import json
import struct
import zlib

R = Path(__file__).resolve().parents[1]
IDS = (80, 280, 380, 480, 580, 680)
EXP_COLUMN = 12
SERVER = R / 'game-data/DATA/MOB_DATA.txt'
CLIENT = R / 'client-overlay/Monster/monster.dat'
BASELINE = R / '.cache/primordial-experience-v14'
REPORT = R / 'assets/primordial-baphomet/experience-validation.json'


def sha(raw):
    return hashlib.sha256(raw).hexdigest()


def rows(raw):
    found = {}
    for line in raw.splitlines():
        cells = line.split(b'\t')
        if not cells[0].isdigit():
            continue
        ident = int(cells[0])
        if ident in found:
            raise ValueError('Duplicate monster ID: ' + str(ident))
        found[ident] = cells
    return found


def desired_exp(server_raw):
    return int(rows(server_raw)[44][EXP_COLUMN]) * 2


def patch_text(raw, experience):
    if not 0 < experience < 2**31:
        raise ValueError('EXP is outside the native signed integer range')
    parsed = rows(raw)
    if not all(ident in parsed for ident in IDS):
        raise ValueError('All six primordial monster rows are required')
    result = []
    for line in raw.splitlines(keepends=True):
        body = line.rstrip(b'\r\n')
        ending = line[len(body):]
        cells = body.split(b'\t')
        if cells[0].isdigit() and int(cells[0]) in IDS:
            if len(cells) != 38:
                raise ValueError('Unexpected primordial monster row width')
            cells[EXP_COLUMN] = str(experience).encode('ascii')
            line = b'\t'.join(cells) + ending
        result.append(line)
    return b''.join(result)


def client_text(raw):
    text = zlib.decompress(raw[20:])
    if len(text) != struct.unpack_from('<I', raw, 16)[0]:
        raise ValueError('Invalid compressed monster table length')
    return text


def patch_client(raw, experience):
    before = client_text(raw)
    text = patch_text(before, experience)
    if text == before:
        return raw
    return raw[:16] + struct.pack('<I', len(text)) + zlib.compress(text)


def verify_delta(before, after, experience):
    if after != patch_text(before, experience):
        raise ValueError('Changed a field or row outside primordial EXP')
    old, new = rows(before), rows(after)
    return [dict(id=i, before=int(old[i][EXP_COLUMN]), after=int(new[i][EXP_COLUMN])) for i in IDS]


def validate():
    server_before = (BASELINE / 'server-before.txt').read_bytes()
    client_before = (BASELINE / 'client-before.dat').read_bytes()
    baseline = json.loads((BASELINE / 'baseline.json').read_text(encoding='utf-8'))
    assert sha(server_before) == baseline['server_before_sha256']
    assert sha(client_before) == baseline['client_before_sha256']
    server, client = SERVER.read_bytes(), CLIENT.read_bytes()
    exp = desired_exp(server_before)
    server_changes = verify_delta(server_before, server, exp)
    client_changes = verify_delta(client_text(client_before), client_text(client), exp)
    assert client[:16] == client_before[:16]
    for path, digest in baseline['protected'].items():
        assert sha(Path(path).read_bytes()) == digest, path
    result = dict(passed=True, base_id=44, base_exp=exp // 2, primordial_exp=exp,
                  column_zero_based=EXP_COLUMN, variant_ids=list(IDS), server_rows=server_changes,
                  client_rows=client_changes, other_rows_fields_and_line_endings_preserved=True,
                  client_header_preserved=True, protected_hashes_unchanged=len(baseline['protected']),
                  server_sha256=sha(server), client_sha256=sha(client),
                  source_loader='src/server/LAQIA_GameServer/monster.cpp:648-663 (_dUMENOKOJI_)',
                  installed_files_written=False, vm_database_or_accounts_written=False)
    REPORT.write_text(json.dumps(result, ensure_ascii=False, indent=2) + '\n', encoding='utf-8')
    return result


def prepare():
    BASELINE.mkdir(parents=True, exist_ok=True)
    if not (BASELINE / 'baseline.json').exists():
        server, client = SERVER.read_bytes(), CLIENT.read_bytes()
        protected = [R / 'game-data/DATA/MobItemLoseTable.txt', R / 'server-bin/LAQIA_GameServer',
                     R / 'runtime/build/LAQIA_GameServer', R / 'client-overlay/UInterface.dll']
        protected.extend(p for p in (R / 'game-data/DATA').glob('*')
                         if p.is_file() and ('exp' in p.name.lower() or 'rate' in p.name.lower()))
        (BASELINE / 'server-before.txt').write_bytes(server)
        (BASELINE / 'client-before.dat').write_bytes(client)
        baseline = dict(server_before_sha256=sha(server), client_before_sha256=sha(client),
                        protected={str(p): sha(p.read_bytes()) for p in protected})
        (BASELINE / 'baseline.json').write_text(json.dumps(baseline, indent=2) + '\n', encoding='utf-8')
    original_server = (BASELINE / 'server-before.txt').read_bytes()
    original_client = (BASELINE / 'client-before.dat').read_bytes()
    exp = desired_exp(original_server)
    server = patch_text(original_server, exp)
    client = patch_client(original_client, exp)
    assert SERVER.read_bytes() in (original_server, server), 'Source server table changed independently'
    assert CLIENT.read_bytes() in (original_client, client), 'Source client table changed independently'
    SERVER.write_bytes(server)
    CLIENT.write_bytes(client)
    return validate()


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--check', action='store_true')
    args = parser.parse_args()
    print(json.dumps(validate() if args.check else prepare(), ensure_ascii=False))

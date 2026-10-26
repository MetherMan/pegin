"""Data for assets/golden-knight/effects.html: the gold sword trail and gold arrow vs the originals.

Reads the actual client files (client-overlay over the base client) with the Skill preview decoders
(client-overlay/Tools/SkillColors/native_assets.py): .wed frame tables, .wem meshes, trace.tdf,
magic scripts and effect textures. Writes assets/golden-knight/effects-preview.json.
"""
from pathlib import Path
import base64, json, sys
R = Path(__file__).resolve().parents[1]
sys.path[:0] = [str(R/'runtime/pylibs'), str(R/'client-overlay/Tools/SkillColors')]
from native_assets import read_wed, read_wem, read_magic, read_traces, png_from_wtm

ROOTS = [R/'client-overlay', R/'runtime/client/GameClient']
SETS = {'gold': dict(script='cw_arrow', trail='celestial_gold'), 'original': dict(script='arw_d1', trail='검흔C')}


def read(rel):
    for root in ROOTS:
        if (root/rel).is_file():
            return (root/rel).read_bytes()
    raise FileNotFoundError(rel)


def main():
    out = dict(fps=30, effects={}, meshes={}, textures={}, traces={}, scripts={})
    traces = read_traces(read('Effect/trace.tdf'))

    def texture(name):
        if name not in out['textures']:
            out['textures'][name] = 'data:image/png;base64,'+base64.b64encode(png_from_wtm(read('Texture/Effect/'+name))).decode()
    for key, spec in SETS.items():
        effects = read_magic(read(f'Magic/{spec["script"]}.ms').decode('cp949'))
        out['scripts'][key] = dict(arrow=effects, trail=spec['trail'])
        names = {e['wed'] for e in effects if e['wed']} | {p['TAIL'] for e in effects for p in e['parts'] if 'TAIL' in p}
        for name in names:
            effect = read_wed(read('Effect/'+name))
            out['effects'][name] = effect
            if effect['mesh'] not in out['meshes']:
                out['meshes'][effect['mesh']] = read_wem(read('Effect/'+effect['mesh']))
            texture(effect['texture'])
        for trace in {e['trace'] for e in effects if e['trace']} | {spec['trail']}:
            out['traces'][trace] = traces[trace]
            texture(traces[trace]['texture'])
    path = R/'assets/golden-knight/effects-preview.json'
    path.write_text(json.dumps(out, ensure_ascii=False, separators=(',', ':')), encoding='utf-8')
    print(json.dumps(dict(effects=sorted(out['effects']), traces=out['traces'],
                          scripts={k: [(e['wed'], e['trace'], len(e['parts'])) for e in v['arrow']] for k, v in out['scripts'].items()}),
                     ensure_ascii=False))


if __name__ == '__main__':
    main()

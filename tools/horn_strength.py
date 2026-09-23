"""Deterministic horn texture contrast shared by build and local preview."""
from pathlib import Path
import io, json, math, os, struct, sys, zlib

R = Path(__file__).resolve().parents[1]
O = R / 'assets/primordial-baphomet'
D = R / 'client-overlay'
SETTINGS = O / 'horn-settings.json'
SOURCE = O / 'horn-satin-keratin-v5-imagegen.png'
CENTER = (39, 35, 31)
sys.path.insert(0, str(R / 'runtime/pylibs'))
from PIL import Image


def validate_strength(value):
    if isinstance(value, bool) or not isinstance(value, (int, float)) or not math.isfinite(value) or not 0 <= value <= 8:
        raise ValueError('뿔 질감 강도는 0~8 사이의 유한한 숫자여야 합니다.')
    return float(value)


def load_strength():
    return validate_strength(json.loads(SETTINGS.read_text(encoding='utf-8')).get('strength')) if SETTINGS.exists() else 1.0


def apply_strength(image, strength):
    strength = validate_strength(strength)
    table = [max(0, min(255, math.floor(center + (value - center) * strength + .5)))
             for center in CENTER for value in range(256)]
    return image.convert('RGB').point(table)


def base_tile(index):
    if index not in (0, 1):
        raise ValueError('Invalid horn tile')
    with Image.open(SOURCE) as image:
        return image.convert('RGB').resize((1024 if index == 0 else 512, 1024), Image.Resampling.LANCZOS)


def png_bytes(image):
    buffer = io.BytesIO()
    image.save(buffer, format='PNG')
    return buffer.getvalue()


def prepared_files(strength):
    strength = validate_strength(strength)
    files = {SETTINGS: (json.dumps({'strength': strength}, indent=2) + '\n').encode()}
    for index, (preview, texture, left_width) in enumerate((
        ('body-cyclops-atlas.png', 'mt_prime_body.wtm', 1024),
        ('cleaver-atlas.png', 'mt_prime_cleaver.wtm', 1536))):
        with Image.open(O / preview) as image:
            atlas = image.convert('RGB')
        assert atlas.size == (2048, 1024)
        left = atlas.crop((0, 0, left_width, 1024)).tobytes()
        atlas.paste(apply_strength(base_tile(index), strength), (left_width, 0))
        assert atlas.crop((0, 0, left_width, 1024)).tobytes() == left
        files[O / preview] = png_bytes(atlas)
        buffer = io.BytesIO()
        atlas.save(buffer, format='BMP')
        bmp = buffer.getvalue()
        files[D / 'Texture/Monster' / texture] = b'TEAMMAY\0\0' + struct.pack('<I', len(bmp)) + zlib.compress(bmp, 9)
    return files


def atomic_write(path, data):
    temporary = path.with_name(path.name + '.horn-strength.tmp')
    try:
        temporary.write_bytes(data)
        os.replace(temporary, path)
    finally:
        if temporary.exists():
            temporary.unlink()

"""Six EXP fields only: byte-preserving data edit, compressed table and guards."""
from pathlib import Path
import sys
import struct
import unittest
import zlib

R = Path(__file__).resolve().parents[1]
sys.dont_write_bytecode = True
sys.path.insert(0, str(R / 'tools'))
from primordial_experience import IDS, patch_text, patch_client, client_text, desired_exp, verify_delta


class PrimordialExperience(unittest.TestCase):
    def setUp(self):
        self.raw = (R / '.cache/primordial-experience-v14/server-before.txt').read_bytes()

    def test_six_experience_fields_only_and_idempotent(self):
        out = patch_text(self.raw, 28656)
        self.assertEqual(desired_exp(self.raw), 28656)
        self.assertEqual(patch_text(out, 28656), out)
        a, b = self.raw.splitlines(keepends=True), out.splitlines(keepends=True)
        changed = [(x, y) for x, y in zip(a, b) if x != y]
        self.assertEqual(len(a), len(b))
        self.assertEqual({int(x.split(b'\t')[0]) for x, _ in changed}, set(IDS))
        for x, y in changed:
            old, new = x.split(b'\t'), y.split(b'\t')
            self.assertEqual([i for i in range(len(old)) if old[i] != new[i]], [12])

    def test_compressed_header_and_non_target_rows_unchanged(self):
        raw = (R / '.cache/primordial-experience-v14/client-before.dat').read_bytes()
        out = patch_client(raw, 28656)
        self.assertEqual(out[:16], raw[:16])
        self.assertEqual(struct.unpack_from('<I', out, 16)[0], len(client_text(out)))
        verify_delta(client_text(raw), client_text(out), 28656)
        self.assertEqual(patch_client(out, 28656), out)

    def test_missing_duplicate_and_out_of_range_rejected(self):
        row = next(x for x in self.raw.splitlines(keepends=True) if x.startswith(b'80\t'))
        for raw in (self.raw.replace(row, b''), self.raw + row):
            with self.assertRaises(ValueError):
                patch_text(raw, 28656)
        for value in (0, -1, 2**31):
            with self.assertRaises(ValueError):
                patch_text(self.raw, value)

    def test_unrelated_change_rejected(self):
        bad = patch_text(self.raw, 28656).replace(b'44\t', b'9044\t', 1)
        with self.assertRaises(ValueError):
            verify_delta(self.raw, bad, 28656)


if __name__ == '__main__':
    unittest.main()

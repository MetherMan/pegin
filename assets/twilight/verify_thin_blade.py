"""Compatibility entry point for the revised thin-rim / solid-body check."""
from pathlib import Path
import runpy,sys
target=Path(__file__).resolve().parents[1]/'twilight-set'
sys.path.insert(0,str(target))
runpy.run_path(str(target/'verify_assets.py'),run_name='__main__')

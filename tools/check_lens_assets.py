#!/usr/bin/env python3
"""Compatibility entry point for the lens image asset validator."""

from __future__ import annotations

import subprocess
import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]


if __name__ == "__main__":
    raise SystemExit(
        subprocess.call(
            [sys.executable, str(ROOT / "tools" / "generate_lens_assets.py"), "--check", *sys.argv[1:]]
        )
    )

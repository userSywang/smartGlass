#!/usr/bin/env python3
"""Validate the lens UI asset manifest.

This is intentionally a checker, not a converter. The converter can reuse the
same assets.json contract later; this script keeps additions and deletions from
silently drifting today.
"""

from __future__ import annotations

import argparse
import glob
import json
from pathlib import Path
from typing import Iterable


ROOT = Path(__file__).resolve().parents[1]
ASSET_DIR = ROOT / "apps" / "lens_react_ui" / "assets"
DEFAULT_MANIFEST = ASSET_DIR / "assets.json"


def iter_entries(manifest: dict) -> Iterable[tuple[str, dict]]:
    for group in ("embedded_ui_assets", "simulation_background_assets"):
        for entry in manifest.get(group, []):
            yield group, entry


def resolve_sources(pattern: str) -> list[Path]:
    return [Path(item) for item in glob.glob(str(ASSET_DIR / pattern))]


def main() -> int:
    parser = argparse.ArgumentParser(description="Validate lens_react_ui image asset manifest.")
    parser.add_argument("--manifest", type=Path, default=DEFAULT_MANIFEST)
    args = parser.parse_args()

    manifest_path = args.manifest.resolve()
    with manifest_path.open("r", encoding="utf-8") as manifest_file:
        manifest = json.load(manifest_file)

    generated_dir = (manifest_path.parent / manifest["generated_c_directory"]).resolve()
    errors: list[str] = []

    for group, entry in iter_entries(manifest):
        source_pattern = entry["source"]
        sources = resolve_sources(source_pattern)
        if not sources:
            errors.append(f"{group}: no source matched {source_pattern}")

        output = generated_dir / entry["output"]
        if not output.exists():
            errors.append(f"{group}: missing generated output {output.relative_to(ROOT)}")

    if errors:
        for error in errors:
            print(f"ERROR: {error}")
        return 1

    print("Lens UI asset manifest is valid.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

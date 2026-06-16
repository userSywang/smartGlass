# smartGlass

Minimal ESP-Brookesia + LVGL SDL project for running only `LensReactUI`.

This project keeps the Brookesia logging/check macros and app lifecycle path, but does not include the multi-app advanced demo set. The startup flow creates a Brookesia phone shell, installs `LensReactUI`, and starts it directly through the Brookesia app event path.

## Features

- Lens-style app carousel with Notes, Camera, Navigation, Music, Prompter, Translate, Settings, and AI Assistant entries.
- Global AI assistant callout from the status bar `小智` button or the `A` key.
- Chinese UI strings rendered with the project-local `smartglass_font_16_cjk` LVGL font for later embedded migration.

## Build

Prerequisites:

- CMake 3.12.4 or newer.
- A C/C++ toolchain that can build SDL/LVGL on Windows.
- SDL2 development package. Point CMake at SDL2 with either `-DSDL2_DIR=...` or the `SMARTGLASS_SDL2_DIR` environment variable.

```bat
set SMARTGLASS_SDL2_DIR=F:\path\to\SDL2-2.x.x\cmake
build_and_run.bat
```

Or manually:

```bat
cmake -S . -B build -DSDL2_DIR=F:\path\to\SDL2-2.x.x\cmake
cmake --build build
build\bin\smartGlass.exe
```

## Notes

- `build/` and `out/` are generated locally and are intentionally ignored by Git.
- `lvgl/`, `lv_drivers/`, and `esp-brookesia/` are kept in the repository because the root CMake project builds against these local source directories.
- Chinese UI text uses `apps/lens_react_ui/smartglass_font_16_cjk.c`, a compact LVGL font generated from SimSun for the current UI strings. Keep this file in the target build when porting to embedded hardware.

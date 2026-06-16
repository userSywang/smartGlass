# smartGlass

Minimal ESP-Brookesia + LVGL SDL project for running only `LensReactUI`.

This project keeps the Brookesia logging/check macros and app lifecycle path, but does not include the multi-app advanced demo set. The startup flow creates a Brookesia phone shell, installs `LensReactUI`, and starts it directly through the Brookesia app event path.

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
- Chinese UI text uses LVGL's built-in `lv_font_simsun_16_cjk` font. Keep `LV_FONT_SIMSUN_16_CJK` enabled in `lv_conf.h` when porting to embedded targets that need the same Chinese strings.

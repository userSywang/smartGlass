# smartGlass

Minimal ESP-Brookesia + LVGL SDL project for running only `LensReactUI`.

This project keeps the Brookesia logging/check macros and app lifecycle path, but does not include the multi-app advanced demo set. The startup flow creates a Brookesia phone shell, installs `LensReactUI`, and starts it directly through the Brookesia app event path.

## Build

```bat
build_and_run.bat
```

Or manually:

```bat
cmake -S . -B build
cmake --build build
build\bin\smartGlass.exe
```

# smartGlass

基于 LVGL 8 和 SDL2 的 AI 眼镜交互模拟器。程序启动后直接进入独立主页面，不依赖手机式后台任务或底部手势导航。

## 功能模块

- 备忘、相机、导航、通知、音乐、提词
- 翻译、实时翻译、双语畅聊、会议记录
- 设置和小智语音助手

当前 `SimulationLensDataProvider` 提供演示数据；LVGL 页面通过 `LensDataProvider` 接口读取内容，便于后续替换设备数据源，但本工程尚未执行真实硬件移植。

## 目录

- `apps/lens_react_ui/*.cpp|*.hpp`：页面、交互、状态和数据接口代码。
- `apps/lens_react_ui/assets/`：可编辑的 PNG/SVG 源资源。
- `apps/lens_react_ui/generated/`：图片和字体生成的 LVGL C 数组。
- `apps/simulator/generated/`：仅供 PC 模拟器使用的生成资源。
- `tests/`：不依赖窗口环境的逻辑测试。
- `tools/`：字体、图片生成和资源校验工具。
- `lvgl/`、`lv_drivers/`：当前构建使用的第三方源码。

## 构建

需要 CMake 3.12.4、C/C++ 工具链和 SDL2 开发包。可以设置：

```bat
set SMARTGLASS_SDL2_DIR=F:\path\to\SDL2-2.x.x\cmake
```

调试并运行：

```bat
build_and_run.bat
```

生成真正的 Release 构建：

```bat
build_release.bat
```

Release 构建在 GNU 工具链下默认剥离符号；需要保留符号时使用：

```bat
cmake -S . -B build\release -DCMAKE_BUILD_TYPE=Release -DSMARTGLASS_STRIP_RELEASE_BINARY=OFF
```

仅评估不含模拟照片背景的构建：

```bat
cmake -S . -B build\release-embedded -DCMAKE_BUILD_TYPE=Release -DSMARTGLASS_ENABLE_SIMULATION_BACKGROUNDS=OFF
cmake --build build\release-embedded --parallel
```

## 测试与资源

```bat
ctest --test-dir build --output-on-failure
python tools\generate_lens_assets.py --check
```

图片生成和目录规则见 `apps/lens_react_ui/assets/README.md`。

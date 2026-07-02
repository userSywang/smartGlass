# Lens React UI 资源管理

资源与实际代码分开管理：

- `assets/ui/`：嵌入式界面仍会使用的 PNG/SVG 源文件。
- `assets/simulation/backgrounds/`：仅用于 PC 模拟器的照片背景源文件。
- `generated/ui/`：由 UI PNG 生成的 LVGL C 数组。
- `generated/simulation/`：模拟背景对应的 LVGL C 数组。
- `generated/fonts/`：字体转换生成的 C 文件。
- `*.cpp`、`*.hpp`：页面、交互、状态和数据提供者代码。

## 图片生成

图片清单统一定义在 `assets.json`。需要 Pillow：

```bash
python -m pip install Pillow
```

生成全部图片数组：

```bash
python tools/generate_lens_assets.py
```

只生成指定资源组：

```bash
python tools/generate_lens_assets.py --group home_icons
```

检查源图、像素、尺寸、符号和生成文件是否同步：

```bash
python tools/generate_lens_assets.py --check
cmake --build build --target check_lens_assets
```

SVG 是设计源文件。当前生成器以 PNG 作为确定性栅格输入，修改 SVG 后应先同步导出同目录 PNG。

## 构建策略

- UI 资源默认编译进程序。
- 模拟背景由 `SMARTGLASS_ENABLE_SIMULATION_BACKGROUNDS` 控制。
- 模拟器默认开启背景。
- 评估嵌入式构建时可使用 `-DSMARTGLASS_ENABLE_SIMULATION_BACKGROUNDS=OFF`。

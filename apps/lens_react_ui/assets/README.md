# Lens React UI 资源管理

本目录只保存图片源文件，`apps/lens_react_ui/*_asset.c` 是由源文件生成的 LVGL C 数组。

## 目录分层

- `ui/`：真实嵌入式设备仍会使用的固定 UI 资源，例如主页面图标、按钮、HUD 图标、麦克风、时钟。
- `simulation/backgrounds/`：仅用于 PC 模拟器增强画面表现的背景图。真实嵌入式设备移植时默认不依赖这些照片级背景。

## 构建策略

- UI 资源默认编译进固件，适合用 C 数组随程序发布。
- 模拟背景通过 CMake 选项 `SMARTGLASS_ENABLE_SIMULATION_BACKGROUNDS` 控制。
- 模拟器默认开启背景：`ON`。
- 嵌入式目标可关闭背景：`-DSMARTGLASS_ENABLE_SIMULATION_BACKGROUNDS=OFF`。

## 后续生成脚本规则

新增或删除图片时，优先更新 `assets.json`，再由生成脚本统一生成对应的 `*_asset.c`。
背景图不要放入 `ui/`，避免真实设备移植时误编译大图资源。

当前可先运行下面的校验脚本，确认源图和已生成 C 数组没有脱节：

```bash
python tools/check_lens_assets.py
```

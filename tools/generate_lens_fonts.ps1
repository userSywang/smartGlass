param(
    [string]$FontPath = ".font-cache/NotoSansCJKsc-Regular.otf",
    [string]$FontUrl = "https://raw.githubusercontent.com/notofonts/noto-cjk/main/Sans/OTF/SimplifiedChinese/NotoSansCJKsc-Regular.otf"
)

$ErrorActionPreference = "Stop"

$repoRoot = Split-Path -Parent $PSScriptRoot
Set-Location $repoRoot

$fontDir = Split-Path -Parent $FontPath
if (-not (Test-Path $fontDir)) {
    New-Item -ItemType Directory -Path $fontDir | Out-Null
}

if (-not (Test-Path $FontPath)) {
    Write-Host "Downloading Noto Sans CJK SC Regular..."
    Invoke-WebRequest -Uri $FontUrl -OutFile $FontPath -UseBasicParsing
}

$sourceFiles = @(
    "apps/lens_react_ui/LensReactUI.cpp",
    "apps/lens_react_ui/LensReactUI.hpp"
)

$chars = [System.Collections.Generic.HashSet[int]]::new()

# Keep printable ASCII available for mixed labels such as "小智 AI 助手" and "UTF-8".
for ($code = 32; $code -le 126; $code++) {
    [void]$chars.Add($code)
}

foreach ($file in $sourceFiles) {
    $text = Get-Content -Raw -Encoding UTF8 $file
    foreach ($rune in $text.EnumerateRunes()) {
        $value = $rune.Value
        if ($value -gt 126) {
            [void]$chars.Add($value)
        }
    }
}

$symbolsBuilder = [System.Text.StringBuilder]::new()
foreach ($code in ($chars | Sort-Object)) {
    [void]$symbolsBuilder.Append([System.Text.Rune]::new($code).ToString())
}
$symbols = $symbolsBuilder.ToString()

$sizes = @(12, 14, 16, 20)
foreach ($size in $sizes) {
    $output = "apps/lens_react_ui/smartglass_font_${size}_cjk.c"
    Write-Host "Generating $output"
    & npx -y lv_font_conv `
        --font $FontPath `
        --size $size `
        --bpp 4 `
        --format lvgl `
        --symbols $symbols `
        --no-compress `
        --output $output `
        --force-fast-kern-format `
        --lv-include lvgl.h `
        --lv-font-name "smartglass_font_${size}_cjk"
}

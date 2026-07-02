cmake_minimum_required(VERSION 3.12.4)

project(lens_react_ui)

option(SMARTGLASS_ENABLE_SIMULATION_BACKGROUNDS "Compile simulation-only background image assets" ON)

set(LENS_UI_CODE_SRCS
    LensReactUI.cpp
    LensReactUICameraPage.cpp
    LensReactUIEvents.cpp
    LensReactUILifecycle.cpp
    LensReactUIMeetingPage.cpp
    LensReactUIMiscPages.cpp
    LensReactUIMusicPage.cpp
    LensReactUINavigationPage.cpp
    LensReactUINotificationPage.cpp
    LensReactUIPrompterPage.cpp
    LensReactUISession.cpp
    LensReactUITranslationPages.cpp
    NavigationSimulation.cpp
    NotificationTimeline.cpp
    SimulationLensDataProvider.cpp
)

set(LENS_UI_ASSET_SRCS
    generated/ui/assistant_bot_asset.c
    generated/ui/camera_shutter_asset.c
    generated/ui/home_icons_asset.c
    generated/ui/meeting_microphone_asset.c
    generated/ui/nav_assets.c
    generated/ui/session_clock_asset.c
)

set(LENS_SIMULATION_BACKGROUND_ASSET_SRCS
    generated/simulation/camera_lifestyle_background_asset.c
    generated/simulation/meeting_background_asset.c
    generated/simulation/navigation_background_asset.c
    generated/simulation/realtime_translate_background_asset.c
)

set(LENS_FONT_SRCS
    generated/fonts/nav_font_72_digits.c
    generated/fonts/smartglass_font_12_cjk.c
    generated/fonts/smartglass_font_14_cjk.c
    generated/fonts/smartglass_font_16_cjk.c
    generated/fonts/smartglass_font_20_cjk.c
)

set(LENS_MANAGED_ASSET_SRCS ${LENS_UI_ASSET_SRCS})
if(SMARTGLASS_ENABLE_SIMULATION_BACKGROUNDS)
    list(APPEND LENS_MANAGED_ASSET_SRCS ${LENS_SIMULATION_BACKGROUND_ASSET_SRCS})
    set(SMARTGLASS_SIMULATION_BACKGROUND_DEFINE 1)
else()
    set(SMARTGLASS_SIMULATION_BACKGROUND_DEFINE 0)
endif()

add_library(${PROJECT_NAME} STATIC
    ${LENS_UI_CODE_SRCS}
    ${LENS_MANAGED_ASSET_SRCS}
    ${LENS_FONT_SRCS}
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} PUBLIC lvgl)

target_compile_definitions(
    ${PROJECT_NAME}
    PRIVATE
    SMARTGLASS_ENABLE_SIMULATION_BACKGROUNDS=${SMARTGLASS_SIMULATION_BACKGROUND_DEFINE}
)

target_compile_options(
    ${PROJECT_NAME}
    PRIVATE
    $<$<COMPILE_LANGUAGE:C>:
        -Wno-format
        -Wno-incompatible-pointer-types
        -Wno-int-conversion
    >
    $<$<COMPILE_LANGUAGE:CXX>:
        -Wno-format
        -Wno-conversion-null
        -Wno-attributes
        -Wno-write-strings
    >
    -DLV_LVGL_H_INCLUDE_SIMPLE
)

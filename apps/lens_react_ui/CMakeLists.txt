cmake_minimum_required(VERSION 3.12.4)

project(lens_react_ui)

option(SMARTGLASS_ENABLE_SIMULATION_BACKGROUNDS "Compile simulation-only background image assets" ON)

file(GLOB_RECURSE APPS_C_SRCS ./*.c)
file(GLOB_RECURSE APPS_CPP_SRCS ./*.cpp)

set(LENS_UI_ASSET_SRCS
    assistant_bot_asset.c
    camera_shutter_asset.c
    home_icons_asset.c
    meeting_microphone_asset.c
    nav_assets.c
    nav_bicycle_icon.c
    nav_flag_icon.c
    session_clock_asset.c
)

set(LENS_SIMULATION_BACKGROUND_ASSET_SRCS
    camera_lifestyle_background_asset.c
    meeting_background_asset.c
    navigation_background_asset.c
    realtime_translate_background_asset.c
)

set(LENS_MANAGED_GENERATED_SRCS
    smartglass_font_16_cjk.c
    ${LENS_UI_ASSET_SRCS}
    ${LENS_SIMULATION_BACKGROUND_ASSET_SRCS}
)

set(APPS_C_SRCS_FILTERED)
foreach(app_src IN LISTS APPS_C_SRCS)
    get_filename_component(app_src_name "${app_src}" NAME)
    if(NOT app_src_name IN_LIST LENS_MANAGED_GENERATED_SRCS)
        list(APPEND APPS_C_SRCS_FILTERED "${app_src}")
    endif()
endforeach()
set(APPS_C_SRCS ${APPS_C_SRCS_FILTERED})

set(LENS_MANAGED_ASSET_SRCS ${LENS_UI_ASSET_SRCS})
if(SMARTGLASS_ENABLE_SIMULATION_BACKGROUNDS)
    list(APPEND LENS_MANAGED_ASSET_SRCS ${LENS_SIMULATION_BACKGROUND_ASSET_SRCS})
    set(SMARTGLASS_SIMULATION_BACKGROUND_DEFINE 1)
else()
    set(SMARTGLASS_SIMULATION_BACKGROUND_DEFINE 0)
endif()

add_library(${PROJECT_NAME} STATIC
    ${APPS_C_SRCS}
    ${APPS_CPP_SRCS}
    ${LENS_MANAGED_ASSET_SRCS}
    smartglass_font_16_cjk.c
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

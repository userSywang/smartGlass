cmake_minimum_required(VERSION 3.12.4)

project(lens_react_ui)

file(GLOB_RECURSE APPS_C_SRCS ./*.c)
file(GLOB_RECURSE APPS_CPP_SRCS ./*.cpp)
list(REMOVE_ITEM APPS_C_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/smartglass_font_16_cjk.c")
list(REMOVE_ITEM APPS_C_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/nav_bicycle_icon.c")
list(REMOVE_ITEM APPS_C_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/nav_flag_icon.c")
list(REMOVE_ITEM APPS_C_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/camera_shutter_asset.c")
list(REMOVE_ITEM APPS_C_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/meeting_microphone_asset.c")
list(REMOVE_ITEM APPS_C_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/meeting_background_asset.c")
list(REMOVE_ITEM APPS_C_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/camera_lifestyle_background_asset.c")
list(REMOVE_ITEM APPS_C_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/home_icons_asset.c")
list(REMOVE_ITEM APPS_C_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/realtime_translate_background_asset.c")
list(REMOVE_ITEM APPS_C_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/navigation_background_asset.c")
list(REMOVE_ITEM APPS_C_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/session_clock_asset.c")

add_library(${PROJECT_NAME} STATIC
    ${APPS_C_SRCS}
    ${APPS_CPP_SRCS}
    nav_bicycle_icon.c
    nav_flag_icon.c
    camera_shutter_asset.c
    meeting_microphone_asset.c
    meeting_background_asset.c
    camera_lifestyle_background_asset.c
    home_icons_asset.c
    realtime_translate_background_asset.c
    navigation_background_asset.c
    session_clock_asset.c
    smartglass_font_16_cjk.c
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} PUBLIC lvgl)

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

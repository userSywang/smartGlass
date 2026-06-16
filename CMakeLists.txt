cmake_minimum_required(VERSION 3.12.4)

project(smartGlass)

set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/out/smartGlass)
set(LV_CONF_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lv_conf.h CACHE STRING "Path defined for lv_conf.h")

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_OBJECT_PATH_MAX 512)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

if((NOT SDL2_DIR OR SDL2_DIR MATCHES "NOTFOUND") AND DEFINED ENV{SMARTGLASS_SDL2_DIR})
    set(SDL2_DIR "$ENV{SMARTGLASS_SDL2_DIR}" CACHE PATH "Path to SDL2Config.cmake" FORCE)
endif()

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_subdirectory(lv_drivers EXCLUDE_FROM_ALL)
add_subdirectory(lvgl EXCLUDE_FROM_ALL)
add_subdirectory(esp-brookesia)
add_subdirectory(apps/lens_react_ui)

add_executable(${PROJECT_NAME}
    main.cpp
    mouse_cursor_icon.c
)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_link_libraries(${PROJECT_NAME} PRIVATE
        pthread
        lens_react_ui
        esp-brookesia
        lv_drivers
        -Wl,--whole-archive
        lvgl
        -Wl,--no-whole-archive
        ${SDL2_STATIC_LIBRARIES}
    )
else()
    target_link_libraries(${PROJECT_NAME} PRIVATE
        pthread
        lens_react_ui
        esp-brookesia
        lv_drivers
        lvgl
        ${SDL2_STATIC_LIBRARIES}
    )
endif()

target_compile_options(${PROJECT_NAME} PRIVATE
    -Wall
    -Wextra
    -Wshadow
    -Wundef
    -Wno-unused-function
    $<$<COMPILE_LANGUAGE:CXX>:
        -Wno-unused-parameter
        -Wno-missing-field-initializers
    >
    -DLV_LVGL_H_INCLUDE_SIMPLE
)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_link_options(${PROJECT_NAME} PRIVATE -static-libstdc++ -static-libgcc)
endif()

set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES
        OUTPUT_NAME ${PROJECT_NAME}
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

install(TARGETS ${PROJECT_NAME} DESTINATION bin)

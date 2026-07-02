cmake_minimum_required(VERSION 3.12.4)

project(smartGlass)

include(CTest)
find_package(Python3 COMPONENTS Interpreter QUIET)

option(SMARTGLASS_STRIP_RELEASE_BINARY "Strip symbols from GNU Release binaries" ON)

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
add_subdirectory(apps/lens_react_ui)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()

if(Python3_Interpreter_FOUND)
    add_custom_target(
        check_lens_assets
        COMMAND ${Python3_EXECUTABLE} ${CMAKE_SOURCE_DIR}/tools/generate_lens_assets.py --check
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Checking generated LVGL image assets"
    )
endif()

add_executable(${PROJECT_NAME}
    main.cpp
    apps/simulator/generated/mouse_cursor_icon.c
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    pthread
    lens_react_ui
    lv_drivers
    lvgl
    ${SDL2_STATIC_LIBRARIES}
)

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
    if(SMARTGLASS_STRIP_RELEASE_BINARY)
        target_link_options(${PROJECT_NAME} PRIVATE $<$<CONFIG:Release>:-s>)
    endif()
endif()

set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES
        OUTPUT_NAME ${PROJECT_NAME}
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

install(TARGETS ${PROJECT_NAME} DESTINATION bin)

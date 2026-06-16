cmake_minimum_required(VERSION 3.12.4)

project(smartGlass)

set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/out/smartGlass)
set(LV_CONF_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lv_conf.h CACHE STRING "Path defined for lv_conf.h")

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_OBJECT_PATH_MAX 512)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_FLAGS "-O0 -g -Wall -Wextra -Wshadow -Wundef -Wno-unused-function")
set(CMAKE_CXX_FLAGS "-O0 -g -Wall -Wextra -Wshadow -Wundef -Wno-unused-function -Wno-unused-parameter -Wno-missing-field-initializers")

set(SMARTGLASS_DEFAULT_SDL2_DIR "F:/360MoveData/Users/Administrator/Desktop/winenv-main/libs/SDL2-2.32.4/cmake")
if((NOT SDL2_DIR OR SDL2_DIR MATCHES "NOTFOUND") AND
        (EXISTS "${SMARTGLASS_DEFAULT_SDL2_DIR}/SDL2Config.cmake" OR EXISTS "${SMARTGLASS_DEFAULT_SDL2_DIR}/sdl2-config.cmake"))
    set(SDL2_DIR "${SMARTGLASS_DEFAULT_SDL2_DIR}" CACHE PATH "Path to SDL2Config.cmake" FORCE)
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

target_link_libraries(${PROJECT_NAME} PRIVATE
    pthread
    lvgl
    lv_drivers
    esp-brookesia
    lens_react_ui
    ${SDL2_STATIC_LIBRARIES}
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")

target_compile_options(${PROJECT_NAME} PRIVATE -DLV_LVGL_H_INCLUDE_SIMPLE)

set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES
        OUTPUT_NAME ${PROJECT_NAME}
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

install(TARGETS ${PROJECT_NAME} DESTINATION bin)

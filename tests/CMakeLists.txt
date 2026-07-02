set(LENS_UI_SOURCE_DIR ${CMAKE_SOURCE_DIR}/apps/lens_react_ui)

function(add_smartglass_logic_test target)
    add_executable(${target} ${ARGN})
    target_include_directories(${target} PRIVATE ${LENS_UI_SOURCE_DIR})
    target_compile_features(${target} PRIVATE cxx_std_17)
    add_test(NAME ${target} COMMAND ${target})
endfunction()

add_smartglass_logic_test(
    navigation_simulation_test
    navigation_simulation_test.cpp
    ${LENS_UI_SOURCE_DIR}/NavigationSimulation.cpp
)

add_smartglass_logic_test(
    notification_timeline_test
    notification_timeline_test.cpp
    ${LENS_UI_SOURCE_DIR}/NotificationTimeline.cpp
)

add_smartglass_logic_test(
    page_runtime_test
    page_runtime_test.cpp
)

add_smartglass_logic_test(
    lens_data_provider_test
    lens_data_provider_test.cpp
    ${LENS_UI_SOURCE_DIR}/SimulationLensDataProvider.cpp
)

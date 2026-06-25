#pragma once

#include <cstdint>

enum class NavigationDirection : uint8_t {
    Straight,
    Left,
    Right,
    SlightLeft,
    SlightRight,
    UTurn,
};

enum class NavigationLightState : uint8_t {
    None,
    Red,
    Green,
};

struct NavigationState {
    int16_t speed = 35;
    int16_t distance = 350;
    int16_t light_distance = 0;
    int8_t light_countdown = 0;
    uint16_t elapsed_seconds = 0;
    NavigationDirection direction = NavigationDirection::Straight;
    NavigationLightState light_state = NavigationLightState::None;
};

void resetNavigationState(NavigationState &state);
void stepNavigationState(NavigationState &state);

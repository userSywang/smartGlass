#include "NavigationSimulation.hpp"

#include <algorithm>

namespace {
constexpr NavigationDirection kDirections[] = {
    NavigationDirection::Straight,
    NavigationDirection::Left,
    NavigationDirection::Right,
    NavigationDirection::SlightLeft,
    NavigationDirection::SlightRight,
    NavigationDirection::UTurn,
    NavigationDirection::Straight,
    NavigationDirection::Straight,
};

int approach(int value, int target, int increase_step, int decrease_step)
{
    if(value < target) {
        return std::min(target, value + increase_step);
    }
    if(value > target) {
        return std::max(target, value - decrease_step);
    }
    return value;
}
}

void resetNavigationState(NavigationState &state)
{
    state = {};
    state.speed = 35;
    state.distance = 350;
    state.direction = NavigationDirection::Straight;
}

void stepNavigationState(NavigationState &state)
{
    ++state.elapsed_seconds;

    if(state.light_state == NavigationLightState::None && state.elapsed_seconds == 18) {
        state.light_state = NavigationLightState::Red;
        state.light_countdown = 16;
        state.light_distance = 80;
    }

    if(state.light_state == NavigationLightState::Red) {
        if(state.light_distance > 0) {
            state.light_distance = std::max(0, state.light_distance - std::max(1, state.speed / 4));
        } else if(state.light_countdown > 0) {
            --state.light_countdown;
        } else {
            state.light_state = NavigationLightState::Green;
            state.light_countdown = 4;
        }
    } else if(state.light_state == NavigationLightState::Green) {
        if(state.light_countdown > 0) {
            --state.light_countdown;
        } else {
            state.light_state = NavigationLightState::None;
        }
    }

    const int cruise_speed = 35 + static_cast<int>(state.elapsed_seconds % 5) - 2;
    int target_speed = cruise_speed;
    if(state.light_state == NavigationLightState::Red) {
        if(state.light_distance == 0) {
            target_speed = 0;
        } else if(state.light_distance < 60) {
            target_speed = std::min(cruise_speed, state.light_distance * 3 / 5);
        }
    }
    state.speed = static_cast<int16_t>(approach(state.speed, target_speed, 4, 8));

    if(state.speed > 0) {
        state.distance -= std::max(1, state.speed / 4);
    }
    if(state.distance <= 0) {
        const uint8_t index = static_cast<uint8_t>((state.elapsed_seconds / 7) %
                                                   (sizeof(kDirections) / sizeof(kDirections[0])));
        state.direction = kDirections[index];
        state.distance = static_cast<int16_t>(150 + (state.elapsed_seconds * 37) % 500);
    }
}

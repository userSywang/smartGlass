#include <cassert>

#include "NavigationSimulation.hpp"

int main()
{
    NavigationState state;
    resetNavigationState(state);
    assert(state.speed == 35);
    assert(state.distance == 350);
    assert(state.light_state == NavigationLightState::None);

    stepNavigationState(state);
    assert(state.distance < 350);
    assert(state.speed >= 30 && state.speed <= 40);

    while(state.elapsed_seconds < 18) {
        stepNavigationState(state);
    }
    assert(state.light_state == NavigationLightState::Red);
    assert(state.light_countdown == 16);
    assert(state.light_distance < 80);

    const int speed_before_braking = state.speed;
    while(state.light_distance > 40) {
        stepNavigationState(state);
    }
    stepNavigationState(state);
    assert(state.speed <= speed_before_braking);

    while(state.light_distance > 0) {
        stepNavigationState(state);
    }
    const int countdown_before = state.light_countdown;
    stepNavigationState(state);
    assert(state.light_countdown == countdown_before - 1);

    while(state.light_state == NavigationLightState::Red) {
        stepNavigationState(state);
    }
    assert(state.light_state == NavigationLightState::Green);

    while(state.light_state == NavigationLightState::Green) {
        stepNavigationState(state);
    }
    assert(state.light_state == NavigationLightState::None);

    state.distance = 1;
    state.speed = 35;
    stepNavigationState(state);
    assert(state.distance >= 150);

    return 0;
}

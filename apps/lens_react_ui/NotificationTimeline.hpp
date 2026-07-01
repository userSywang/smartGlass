#pragma once

#include <cstdint>

constexpr uint16_t kNotificationTimelineTickMs = 100;

enum class NotificationPhase : uint8_t {
    Entering,
    Visible,
    Fading,
    Exiting,
};

struct NotificationTimeline {
    uint16_t age_ms = 0;
    uint16_t phase_ms = 0;
    NotificationPhase phase = NotificationPhase::Entering;
    bool removable = false;
};

void resetNotificationTimeline(NotificationTimeline &timeline);
void stepNotificationTimeline(NotificationTimeline &timeline, uint16_t elapsed_ms);
void forceExitNotificationTimeline(NotificationTimeline &timeline);

#include "NotificationTimeline.hpp"

#include <algorithm>

namespace {
constexpr uint16_t kEnterDurationMs = 200;
constexpr uint16_t kFadeStartMs = 9000;
constexpr uint16_t kExitStartMs = 10000;
constexpr uint16_t kExitDurationMs = 240;
}

void resetNotificationTimeline(NotificationTimeline &timeline)
{
    timeline = {};
}

void forceExitNotificationTimeline(NotificationTimeline &timeline)
{
    if(timeline.phase == NotificationPhase::Exiting) {
        return;
    }
    timeline.phase = NotificationPhase::Exiting;
    timeline.phase_ms = 0;
}

void stepNotificationTimeline(NotificationTimeline &timeline, uint16_t elapsed_ms)
{
    if(timeline.removable) {
        return;
    }

    timeline.age_ms = static_cast<uint16_t>(std::min<uint32_t>(UINT16_MAX, timeline.age_ms + elapsed_ms));
    timeline.phase_ms = static_cast<uint16_t>(std::min<uint32_t>(UINT16_MAX, timeline.phase_ms + elapsed_ms));

    if(timeline.phase == NotificationPhase::Exiting) {
        timeline.removable = timeline.phase_ms >= kExitDurationMs;
        return;
    }
    if(timeline.age_ms >= kExitStartMs) {
        forceExitNotificationTimeline(timeline);
        return;
    }
    if(timeline.age_ms >= kFadeStartMs) {
        timeline.phase = NotificationPhase::Fading;
        timeline.phase_ms = timeline.age_ms - kFadeStartMs;
        return;
    }
    if(timeline.phase == NotificationPhase::Entering && timeline.age_ms >= kEnterDurationMs) {
        timeline.phase = NotificationPhase::Visible;
        timeline.phase_ms = 0;
    }
}

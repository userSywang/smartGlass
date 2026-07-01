#include <cassert>

#include "NotificationTimeline.hpp"

int main()
{
    NotificationTimeline timeline;
    resetNotificationTimeline(timeline);
    assert(timeline.phase == NotificationPhase::Entering);

    stepNotificationTimeline(timeline, 200);
    assert(timeline.phase == NotificationPhase::Visible);

    stepNotificationTimeline(timeline, 8800);
    assert(timeline.phase == NotificationPhase::Fading);

    stepNotificationTimeline(timeline, 1000);
    assert(timeline.phase == NotificationPhase::Exiting);

    stepNotificationTimeline(timeline, 200);
    assert(!timeline.removable);
    stepNotificationTimeline(timeline, 40);
    assert(timeline.removable);

    resetNotificationTimeline(timeline);
    forceExitNotificationTimeline(timeline);
    assert(timeline.phase == NotificationPhase::Exiting);
    stepNotificationTimeline(timeline, 240);
    assert(timeline.removable);

    resetNotificationTimeline(timeline);
    for(uint16_t elapsed = 0; elapsed < 10000; elapsed += kNotificationTimelineTickMs) {
        stepNotificationTimeline(timeline, kNotificationTimelineTickMs);
    }
    assert(timeline.phase == NotificationPhase::Exiting);
    assert(!timeline.removable);
    for(uint16_t elapsed = 0; elapsed < 300; elapsed += kNotificationTimelineTickMs) {
        stepNotificationTimeline(timeline, kNotificationTimelineTickMs);
    }
    assert(timeline.removable);

    return 0;
}

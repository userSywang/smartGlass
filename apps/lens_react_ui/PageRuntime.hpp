#pragma once

#include <cstdint>

enum class PageTimerKind : uint8_t {
    None,
    Camera,
    Navigation,
    Notification,
    Music,
    Prompter,
    Translate,
    RealtimeTranslate,
    BilingualChat,
    Meeting,
};

constexpr PageTimerKind pageTimerForApp(int8_t app_index)
{
    switch(app_index) {
    case 1:
        return PageTimerKind::Camera;
    case 2:
        return PageTimerKind::Navigation;
    case 3:
        return PageTimerKind::Notification;
    case 4:
        return PageTimerKind::Music;
    case 5:
        return PageTimerKind::Prompter;
    case 6:
        return PageTimerKind::Translate;
    case 7:
        return PageTimerKind::RealtimeTranslate;
    case 8:
        return PageTimerKind::BilingualChat;
    case 9:
        return PageTimerKind::Meeting;
    default:
        return PageTimerKind::None;
    }
}

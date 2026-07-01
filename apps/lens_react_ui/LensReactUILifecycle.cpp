#include "LensReactUI.hpp"

void LensReactUI::pausePageTimers(void)
{
    lv_timer_t *const timers[] = {
        _prompt_timer,
        _navigation_timer,
        _notification_timer,
        _translate_timer,
        _realtime_translate_timer,
        _bilingual_timer,
        _meeting_timer,
        _music_timer,
        _camera_timer,
    };

    for(lv_timer_t *timer : timers) {
        if(timer != nullptr) {
            lv_timer_pause(timer);
        }
    }
}

void LensReactUI::resumePageTimer(int8_t app_index)
{
    lv_timer_t *timer = nullptr;
    switch(pageTimerForApp(app_index)) {
    case PageTimerKind::Camera:
        timer = _camera_timer;
        break;
    case PageTimerKind::Navigation:
        timer = _navigation_timer;
        break;
    case PageTimerKind::Notification:
        timer = _notification_timer;
        break;
    case PageTimerKind::Music:
        timer = _music_timer;
        break;
    case PageTimerKind::Prompter:
        timer = _prompt_timer;
        break;
    case PageTimerKind::Translate:
        timer = _translate_timer;
        break;
    case PageTimerKind::RealtimeTranslate:
        timer = _realtime_translate_timer;
        break;
    case PageTimerKind::BilingualChat:
        timer = _bilingual_timer;
        break;
    case PageTimerKind::Meeting:
        timer = _meeting_timer;
        break;
    case PageTimerKind::None:
        break;
    }

    if(timer != nullptr) {
        lv_timer_resume(timer);
    }
}

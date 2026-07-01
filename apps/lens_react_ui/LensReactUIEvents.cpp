#include "LensUIInternal.hpp"

using namespace lens_ui;

void LensReactUI::onClockTimer(lv_timer_t *timer)
{
    auto *app = static_cast<LensReactUI *>(timer ? LENS_TIMER_USER_DATA(timer) : nullptr);
    if(app) {
        app->updateClock();
    }
}

void LensReactUI::onPromptTimer(lv_timer_t *timer)
{
    auto *app = static_cast<LensReactUI *>(timer ? LENS_TIMER_USER_DATA(timer) : nullptr);
    if(app == nullptr || app->_current_app != 5) {
        return;
    }
    const LensDataView<const char *> segments = app->_data_provider.prompterSegments();
    if(segments.size == 0) {
        return;
    }
    app->_prompt_focus = static_cast<uint8_t>((app->_prompt_focus + 1) % segments.size);
    app->updatePrompterPage();
}

void LensReactUI::onMicTimer(lv_timer_t *timer)
{
    auto *app = static_cast<LensReactUI *>(timer ? LENS_TIMER_USER_DATA(timer) : nullptr);
    if(app == nullptr) {
        return;
    }
    if(app->_assistant_overlay) {
        constexpr lv_coord_t patterns[6][5] = {
            {8, 16, 24, 16, 10},
            {14, 24, 12, 20, 8},
            {20, 10, 18, 26, 14},
            {10, 20, 28, 14, 22},
            {18, 26, 14, 22, 10},
            {12, 18, 22, 10, 18},
        };
        app->_assistant_wave_step = (app->_assistant_wave_step + 1) % 6;
        for(uint8_t i = 0; i < 5; ++i) {
            lv_obj_t *bar = app->_assistant_wave_bars[i];
            if(bar == nullptr) {
                continue;
            }
            lv_obj_set_height(bar, patterns[app->_assistant_wave_step][i]);
            lv_obj_align(bar, LV_ALIGN_BOTTOM_MID, (static_cast<int>(i) - 2) * 8, -24);
        }
        return;
    }
    if(app->_mic_ring == nullptr) {
        return;
    }
    app->_mic_expand = !app->_mic_expand;
    const bool compact = app->_view_height <= kCompactViewMaxHeight;
    const lv_coord_t ring_size = compact ? (app->_mic_expand ? 48 : 42) : (app->_mic_expand ? 88 : 70);
    lv_obj_set_size(app->_mic_ring, ring_size, ring_size);
    lv_obj_set_style_bg_opa(app->_mic_ring, app->_mic_expand ? LV_OPA_10 : LV_OPA_30, 0);
    lv_obj_align(app->_mic_ring, compact ? LV_ALIGN_CENTER : LV_ALIGN_BOTTOM_MID, 0, compact ? 1 : -70);
}

void LensReactUI::onNavigationTimer(lv_timer_t *timer)
{
    auto *app = static_cast<LensReactUI *>(timer ? LENS_TIMER_USER_DATA(timer) : nullptr);
    if(app == nullptr || app->_current_app != 2) {
        return;
    }
    stepNavigationState(app->_navigation_state);
    app->updateNavigationPage();
}

void LensReactUI::onNotificationTimer(lv_timer_t *timer)
{
    auto *app = static_cast<LensReactUI *>(timer ? LENS_TIMER_USER_DATA(timer) : nullptr);
    if(app == nullptr || app->_current_app != kNotificationAppIndex) {
        return;
    }
    for(uint8_t i = 0; i < kMaxNotificationBubbles;) {
        auto &item = app->_notification_bubbles[i];
        if(item.obj == nullptr) {
            ++i;
            continue;
        }
        const NotificationPhase previous_phase = item.timeline.phase;
        stepNotificationTimeline(item.timeline, kNotificationTimelineTickMs);
        if(previous_phase != item.timeline.phase) {
            if(item.timeline.phase == NotificationPhase::Fading) {
                app->startNotificationBubbleFade(i);
            } else if(item.timeline.phase == NotificationPhase::Exiting) {
                app->startNotificationBubbleExit(i);
            }
        }
        if(item.timeline.removable) {
            app->removeNotificationBubble(i);
            continue;
        }
        ++i;
    }
}

void LensReactUI::onTranslateTimer(lv_timer_t *timer)
{
    auto *app = static_cast<LensReactUI *>(timer ? LENS_TIMER_USER_DATA(timer) : nullptr);
    if(app == nullptr || app->_current_app != kTranslateAppIndex) {
        return;
    }
    for(uint8_t i = 1; i < kMaxTranslateItems;) {
        auto &item = app->_translate_items[i];
        if(item.obj == nullptr) {
            ++i;
            continue;
        }
        stepNotificationTimeline(item.timeline, kNotificationTimelineTickMs);
        if(item.timeline.phase == NotificationPhase::Fading) {
            lv_obj_set_style_opa(item.obj, LV_OPA_30, 0);
        }
        if(item.timeline.removable) {
            app->removeTranslateItem(i);
            continue;
        }
        ++i;
    }
}

void LensReactUI::onRealtimeTranslateTimer(lv_timer_t *timer)
{
    auto *app = static_cast<LensReactUI *>(timer ? LENS_TIMER_USER_DATA(timer) : nullptr);
    if(app == nullptr || app->_current_app != kRealtimeTranslateAppIndex) {
        return;
    }
    app->updateRealtimeTranslatePage();
}

void LensReactUI::onBilingualChatTimer(lv_timer_t *timer)
{
    auto *app = static_cast<LensReactUI *>(timer ? LENS_TIMER_USER_DATA(timer) : nullptr);
    if(app == nullptr || app->_current_app != kBilingualChatAppIndex) {
        return;
    }
    app->updateBilingualChatTyping();
}

void LensReactUI::onMeetingTimer(lv_timer_t *timer)
{
    auto *app = static_cast<LensReactUI *>(timer ? LENS_TIMER_USER_DATA(timer) : nullptr);
    if(app == nullptr || app->_current_app != kMeetingAppIndex) {
        return;
    }
    app->_meeting_step++;
    app->updateMeetingPage();
}

void LensReactUI::onMusicTimer(lv_timer_t *timer)
{
    auto *app = static_cast<LensReactUI *>(timer ? LENS_TIMER_USER_DATA(timer) : nullptr);
    if(app == nullptr || app->_current_app != kMusicAppIndex || !app->_music_playing) {
        return;
    }
    const LensDataView<MusicTrackSample> tracks = app->_data_provider.musicTracks();
    if(tracks.size == 0) {
        return;
    }
    const MusicTrackSample &track = tracks[app->_music_track_index % tracks.size];
    if(app->_music_elapsed_seconds + 1 >= track.duration_seconds) {
        app->changeMusicTrack(1);
        return;
    }
    app->_music_elapsed_seconds++;
    app->updateMusicPage();
}

void LensReactUI::onCameraTimer(lv_timer_t *timer)
{
    auto *app = static_cast<LensReactUI *>(timer ? LENS_TIMER_USER_DATA(timer) : nullptr);
    if(app == nullptr || app->_current_app != kCameraAppIndex) {
        return;
    }
    if(app->_camera_recording) {
        app->_camera_record_seconds++;
        app->updateCameraPage();
    }
    if(app->_camera_toast_remaining_ms > 0) {
        app->_camera_toast_remaining_ms =
            app->_camera_toast_remaining_ms > 1000 ? app->_camera_toast_remaining_ms - 1000 : 0;
        if(app->_camera_toast_remaining_ms == 0 && app->_camera_toast &&
           !lv_obj_has_flag(app->_camera_toast, LV_OBJ_FLAG_HIDDEN)) {
            lv_anim_del(app->_camera_toast, onAnimOpa);
            lv_anim_t fade;
            lv_anim_init(&fade);
            lv_anim_set_var(&fade, app->_camera_toast);
            lv_anim_set_values(&fade, lv_obj_get_style_opa(app->_camera_toast, 0), LV_OPA_TRANSP);
            lv_anim_set_time(&fade, 220);
            lv_anim_set_path_cb(&fade, lv_anim_path_ease_in);
            lv_anim_set_exec_cb(&fade, onAnimOpa);
            lv_anim_set_ready_cb(&fade, onAnimHideReady);
            lv_anim_start(&fade);
        }
    }
}

void LensReactUI::onRootPressed(lv_event_t *event)
{
    auto *app = static_cast<LensReactUI *>(lv_event_get_user_data(event));
    if(app == nullptr || app->_current_app >= 0) {
        return;
    }
    lv_indev_t *indev = lv_indev_get_act();
    lv_point_t point = {};
    if(indev) {
        lv_indev_get_point(indev, &point);
        app->_press_x = point.x;
        app->_dragging = true;
        app->_suppress_click = false;
    }
}

void LensReactUI::onRootReleased(lv_event_t *event)
{
    auto *app = static_cast<LensReactUI *>(lv_event_get_user_data(event));
    if(app == nullptr || app->_current_app >= 0 || !app->_dragging) {
        return;
    }
    app->_dragging = false;
    lv_indev_t *indev = lv_indev_get_act();
    lv_point_t point = {};
    if(indev == nullptr) {
        return;
    }
    lv_indev_get_point(indev, &point);
    const lv_coord_t diff = app->_press_x - point.x;
    if(diff > 40 && app->_selected_index + 1 < kAppCount) {
        app->_suppress_click = true;
        app->selectIndex(app->_selected_index + 1, true);
    } else if(diff < -40 && app->_selected_index > 0) {
        app->_suppress_click = true;
        app->selectIndex(app->_selected_index - 1, true);
    }
}

void LensReactUI::onRootKey(lv_event_t *event)
{
    auto *app = static_cast<LensReactUI *>(lv_event_get_user_data(event));
    if(app == nullptr) {
        return;
    }
    const uint32_t key = lv_event_get_key(event);
    if(key == kAssistantWakeKey || key == 'a' || key == 'A') {
        app->showAssistantOverlay();
        return;
    }
    if(app->_assistant_overlay) {
        if(key == LV_KEY_ESC || key == LV_KEY_BACKSPACE || key == LV_KEY_ENTER) {
            app->hideAssistantOverlay();
        }
        return;
    }
    if(app->_current_app == kNotificationAppIndex) {
        if(key == '1') {
            app->triggerNotification(0);
            return;
        }
        if(key == '2') {
            app->triggerNotification(1);
            return;
        }
        if(key == '3') {
            app->triggerNotification(2);
            return;
        }
    }
    if(app->_current_app == kTranslateAppIndex) {
        if(key >= '1' && key <= '3') {
            app->triggerTranslation(static_cast<uint8_t>(key - '1'));
            return;
        }
    }
    if(app->_current_app == kBilingualChatAppIndex) {
        if(key == '1') {
            app->triggerBilingualChat(0);
            return;
        }
        if(key == '2') {
            app->triggerBilingualChat(1);
            return;
        }
    }
    if(app->_current_app == kMusicAppIndex) {
        if(key == LV_KEY_LEFT) {
            app->changeMusicTrack(-1);
            return;
        }
        if(key == LV_KEY_RIGHT) {
            app->changeMusicTrack(1);
            return;
        }
        if(key == LV_KEY_ENTER || key == ' ') {
            app->toggleMusicPlayback();
            return;
        }
    }
    if(app->_current_app == kCameraAppIndex) {
        if(key == LV_KEY_ENTER) {
            app->captureCameraPhoto();
            return;
        }
        if(key == ' ') {
            if(app->_camera_recording) {
                app->finishCameraRecording();
            } else {
                app->startCameraRecording();
            }
            return;
        }
    }
    if(app->_current_app < 0) {
        if((key == LV_KEY_RIGHT) && (app->_selected_index + 1 < kAppCount)) {
            app->selectIndex(app->_selected_index + 1, true);
        } else if((key == LV_KEY_LEFT) && (app->_selected_index > 0)) {
            app->selectIndex(app->_selected_index - 1, true);
        } else if(key == LV_KEY_ENTER) {
            app->showApp(app->_selected_index);
        }
    } else if(key == LV_KEY_ESC || key == LV_KEY_BACKSPACE) {
        app->showHome();
    }
}

void LensReactUI::onCameraGesture(lv_event_t *event)
{
    auto *app = static_cast<LensReactUI *>(lv_event_get_user_data(event));
    if(app == nullptr || app->_current_app != kCameraAppIndex) {
        return;
    }
    const lv_event_code_t code = lv_event_get_code(event);
    if(code == LV_EVENT_LONG_PRESSED) {
        app->startCameraRecording();
        return;
    }
    if(code == LV_EVENT_RELEASED) {
        if(app->_camera_recording) {
            app->finishCameraRecording();
        }
        return;
    }
    if(code == LV_EVENT_SHORT_CLICKED) {
        const uint32_t now = lv_tick_get();
        if(app->_camera_last_click_ms != 0 && now - app->_camera_last_click_ms <= 450) {
            app->_camera_last_click_ms = 0;
            app->captureCameraPhoto();
        } else {
            app->_camera_last_click_ms = now;
        }
    }
}

void LensReactUI::onMusicControlClicked(lv_event_t *event)
{
    auto *app = static_cast<LensReactUI *>(lv_event_get_user_data(event));
    if(app == nullptr) {
        return;
    }
    lv_obj_t *target = lv_event_get_target(event);
    const uintptr_t action = reinterpret_cast<uintptr_t>(lv_obj_get_user_data(target));
    if(action == 0) {
        app->changeMusicTrack(-1);
    } else if(action == 1) {
        app->toggleMusicPlayback();
    } else if(action == 2) {
        app->changeMusicTrack(1);
    }
}

void LensReactUI::onAssistantClicked(lv_event_t *event)
{
    auto *app = static_cast<LensReactUI *>(lv_event_get_user_data(event));
    if(app) {
        app->showAssistantOverlay();
    }
}

void LensReactUI::onOverlayDismissed(lv_event_t *event)
{
    auto *app = static_cast<LensReactUI *>(lv_event_get_user_data(event));
    if(app) {
        app->hideAssistantOverlay();
    }
}

void LensReactUI::onTileClicked(lv_event_t *event)
{
    auto *app = static_cast<LensReactUI *>(lv_event_get_user_data(event));
    lv_obj_t *target = lv_event_get_target(event);
    if(app == nullptr || target == nullptr) {
        return;
    }
    if(app->_suppress_click) {
        app->_suppress_click = false;
        return;
    }
    const uint8_t index = static_cast<uint8_t>(reinterpret_cast<uintptr_t>(lv_obj_get_user_data(target)));
    if(index == app->_selected_index) {
        app->showApp(index);
    } else {
        app->selectIndex(index, true);
    }
}

void LensReactUI::onAnimX(void *obj, int32_t value)
{
    lv_obj_set_x(static_cast<lv_obj_t *>(obj), value);
}

void LensReactUI::onAnimY(void *obj, int32_t value)
{
    lv_obj_set_y(static_cast<lv_obj_t *>(obj), value);
}

void LensReactUI::onAnimWidth(void *obj, int32_t value)
{
    lv_obj_set_width(static_cast<lv_obj_t *>(obj), value);
}

void LensReactUI::onAnimOpa(void *obj, int32_t value)
{
    lv_obj_set_style_opa(static_cast<lv_obj_t *>(obj), (lv_opa_t)value, 0);
}

void LensReactUI::onAnimHideReady(lv_anim_t *anim)
{
    auto *obj = static_cast<lv_obj_t *>(anim ? anim->var : nullptr);
    if(obj == nullptr || !lv_obj_is_valid(obj)) {
        return;
    }
    lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
    lv_obj_set_style_opa(obj, LV_OPA_COVER, 0);
}

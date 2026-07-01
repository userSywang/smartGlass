#pragma once

#include <cstdint>

#include "lvgl.h"
#include "NavigationSimulation.hpp"
#include "NotificationTimeline.hpp"
#include "PageRuntime.hpp"

class LensReactUI {
public:
    LensReactUI();
    ~LensReactUI() = default;

    bool init(void);
    bool run(void);
    bool back(void);
    bool close(void);

private:
    static constexpr uint8_t kAppCount = 12;
    static constexpr uint8_t kCameraAppIndex = 1;
    static constexpr uint8_t kNotificationAppIndex = 3;
    static constexpr uint8_t kMusicAppIndex = 4;
    static constexpr uint8_t kPrompterAppIndex = 5;
    static constexpr uint8_t kMaxNotificationBubbles = 4;
    static constexpr uint8_t kTranslateAppIndex = 6;
    static constexpr uint8_t kMaxTranslateItems = 3;
    static constexpr uint8_t kRealtimeTranslateAppIndex = 7;
    static constexpr uint8_t kBilingualChatAppIndex = 8;
    static constexpr uint8_t kMaxBilingualChatItems = 6;
    static constexpr uint8_t kMeetingAppIndex = 9;
    static constexpr uint8_t kAssistantAppIndex = 11;

    static void onClockTimer(lv_timer_t *timer);
    static void onPromptTimer(lv_timer_t *timer);
    static void onMicTimer(lv_timer_t *timer);
    static void onNavigationTimer(lv_timer_t *timer);
    static void onNotificationTimer(lv_timer_t *timer);
    static void onTranslateTimer(lv_timer_t *timer);
    static void onRealtimeTranslateTimer(lv_timer_t *timer);
    static void onBilingualChatTimer(lv_timer_t *timer);
    static void onMeetingTimer(lv_timer_t *timer);
    static void onMusicTimer(lv_timer_t *timer);
    static void onCameraTimer(lv_timer_t *timer);
    static void onRootPressed(lv_event_t *event);
    static void onRootReleased(lv_event_t *event);
    static void onRootKey(lv_event_t *event);
    static void onTileClicked(lv_event_t *event);
    static void onAssistantClicked(lv_event_t *event);
    static void onMusicControlClicked(lv_event_t *event);
    static void onCameraGesture(lv_event_t *event);
    static void onOverlayDismissed(lv_event_t *event);
    static void onAnimX(void *obj, int32_t value);
    static void onAnimY(void *obj, int32_t value);
    static void onAnimWidth(void *obj, int32_t value);
    static void onAnimOpa(void *obj, int32_t value);
    static void onAnimHideReady(lv_anim_t *anim);

    struct NotificationBubble {
        lv_obj_t *obj = nullptr;
        NotificationTimeline timeline = {};
        bool fade_started = false;
        bool exit_started = false;
    };

    struct TranslateItem {
        lv_obj_t *obj = nullptr;
        NotificationTimeline timeline = {};
    };

    struct BilingualChatItem {
        lv_obj_t *obj = nullptr;
        lv_obj_t *primary_label = nullptr;
        lv_obj_t *secondary_label = nullptr;
        const char *primary_text = nullptr;
        const char *secondary_text = nullptr;
        uint16_t primary_total = 0;
        uint16_t secondary_total = 0;
        uint16_t primary_progress = 0;
        uint16_t secondary_progress = 0;
        uint8_t typing_stage = 3;
        uint8_t typing_delay = 0;
        bool mine = false;
    };

    void createHome(lv_coord_t width, lv_coord_t height);
    void createStatusBar(lv_coord_t width, lv_coord_t height);
    void createPageHost(lv_coord_t width, lv_coord_t height);
    void rebuildPage(void);
    void showHome(void);
    void showApp(uint8_t index);
    void showAssistantOverlay(void);
    void hideAssistantOverlay(void);
    void selectIndex(uint8_t index, bool animate);
    void updateTileStyles(void);
    void updateClock(void);
    void updateDots(void);
    void updateNavigationPage(void);
    void updatePrompterPage(void);
    void createSessionTimerPill(lv_align_t align, lv_coord_t x, lv_coord_t y);
    void resetSessionTimer(void);
    void updateSessionTimer(void);
    void clearPageContent(void);
    void pausePageTimers(void);
    void resumePageTimer(int8_t app_index);
    void clearNotificationBubbles(void);
    void triggerNotification(uint8_t type);
    void addNotificationBubble(const char *source, const char *sender, const char *message, const char *icon,
                               lv_color_t accent);
    void startNotificationBubbleFade(uint8_t index);
    void startNotificationBubbleExit(uint8_t index);
    void removeNotificationBubble(uint8_t index);
    void updateNotificationBubbleLayout(void);
    void clearTranslateItems(void);
    void triggerTranslation(uint8_t type);
    void addTranslateItem(const char *zh, const char *en);
    void removeTranslateItem(uint8_t index);
    void updateTranslateItemLayout(void);
    void updateRealtimeTranslatePage(void);
    void clearBilingualChatItems(void);
    void triggerBilingualChat(uint8_t type);
    void addBilingualChatItem(const char *role, const char *primary, const char *secondary, bool mine);
    void finishBilingualChatTyping(void);
    void updateBilingualChatTyping(void);
    void trimBilingualChatOverflow(void);
    void removeBilingualChatItem(uint8_t index);
    void updateBilingualChatLayout(void);
    void updateMeetingPage(void);
    void updateMusicPage(void);
    void toggleMusicPlayback(void);
    void changeMusicTrack(int8_t delta);
    void updateCameraPage(void);
    void captureCameraPhoto(void);
    void startCameraRecording(void);
    void finishCameraRecording(void);
    void showCameraToast(const char *text);

    void createCameraPage(void);
    void createNotesPage(void);
    void createNavigationPage(void);
    void createNotificationPage(void);
    void createMusicPage(void);
    void createPrompterPage(void);
    void createTranslatePage(void);
    void createRealtimeTranslatePage(void);
    void createBilingualChatPage(void);
    void createMeetingPage(void);
    void createSettingsPage(void);
    void createAssistantPage(void);

    lv_obj_t *_root = nullptr;
    lv_obj_t *_viewport = nullptr;
    lv_obj_t *_home = nullptr;
    lv_obj_t *_track = nullptr;
    lv_obj_t *_status_bar = nullptr;
    lv_obj_t *_dot_row = nullptr;
    lv_obj_t *_assistant_button = nullptr;
    lv_obj_t *_tiles[kAppCount] = {};
    lv_obj_t *_tile_icons[kAppCount] = {};
    lv_obj_t *_tile_names[kAppCount] = {};
    lv_obj_t *_dots[kAppCount] = {};
    lv_obj_t *_page = nullptr;
    lv_obj_t *_page_content = nullptr;
    lv_obj_t *_clock_label = nullptr;
    lv_obj_t *_battery_label = nullptr;
    lv_obj_t *_battery_fill = nullptr;
    lv_obj_t *_assistant_overlay = nullptr;
    lv_obj_t *_assistant_panel = nullptr;
    lv_obj_t *_assistant_wave_bars[5] = {};
    lv_obj_t *_nav_direction_img = nullptr;
    lv_obj_t *_nav_distance_label = nullptr;
    lv_obj_t *_nav_speed_label = nullptr;
    lv_obj_t *_nav_baseline = nullptr;
    lv_obj_t *_nav_start_dot = nullptr;
    lv_obj_t *_nav_end_dot = nullptr;
    lv_obj_t *_nav_traffic_card = nullptr;
    lv_obj_t *_nav_traffic_img = nullptr;
    lv_obj_t *_nav_traffic_title = nullptr;
    lv_obj_t *_nav_traffic_countdown = nullptr;
    lv_obj_t *_notification_stack = nullptr;
    lv_obj_t *_notification_empty_label = nullptr;
    lv_obj_t *_prompt_text_label = nullptr;
    lv_obj_t *_prompt_cursor = nullptr;
    lv_obj_t *_session_timer_pill = nullptr;
    lv_obj_t *_session_elapsed_label = nullptr;
    lv_obj_t *_mic_ring = nullptr;
    NotificationBubble _notification_bubbles[kMaxNotificationBubbles] = {};
    TranslateItem _translate_items[kMaxTranslateItems] = {};
    BilingualChatItem _bilingual_items[kMaxBilingualChatItems] = {};
    lv_obj_t *_translate_stack = nullptr;
    lv_obj_t *_translate_empty = nullptr;
    lv_obj_t *_realtime_source_label = nullptr;
    lv_obj_t *_realtime_translation_label = nullptr;
    lv_obj_t *_bilingual_stack = nullptr;
    lv_obj_t *_bilingual_empty = nullptr;
    lv_obj_t *_meeting_status_pill = nullptr;
    lv_obj_t *_meeting_status_label = nullptr;
    lv_obj_t *_meeting_transcript_pill = nullptr;
    lv_obj_t *_meeting_transcript_content = nullptr;
    lv_obj_t *_meeting_mic_icon = nullptr;
    lv_obj_t *_meeting_transcript_label = nullptr;
    lv_obj_t *_meeting_cards[3] = {};
    lv_obj_t *_music_cover = nullptr;
    lv_obj_t *_music_title = nullptr;
    lv_obj_t *_music_artist = nullptr;
    lv_obj_t *_music_elapsed_label = nullptr;
    lv_obj_t *_music_duration_label = nullptr;
    lv_obj_t *_music_progress_fill = nullptr;
    lv_obj_t *_music_play_icon = nullptr;
    lv_obj_t *_camera_recording_pill = nullptr;
    lv_obj_t *_camera_recording_label = nullptr;
    lv_obj_t *_camera_toast = nullptr;
    lv_obj_t *_camera_toast_label = nullptr;

    lv_timer_t *_clock_timer = nullptr;
    lv_timer_t *_prompt_timer = nullptr;
    lv_timer_t *_mic_timer = nullptr;
    lv_timer_t *_navigation_timer = nullptr;
    lv_timer_t *_notification_timer = nullptr;
    lv_timer_t *_translate_timer = nullptr;
    lv_timer_t *_realtime_translate_timer = nullptr;
    lv_timer_t *_bilingual_timer = nullptr;
    lv_timer_t *_meeting_timer = nullptr;
    lv_timer_t *_music_timer = nullptr;
    lv_timer_t *_camera_timer = nullptr;

    lv_coord_t _width = 0;
    lv_coord_t _height = 0;
    lv_coord_t _view_width = 640;
    lv_coord_t _view_height = 200;
    lv_coord_t _item_step = 128;
    lv_coord_t _press_x = 0;
    uint8_t _selected_index = 1;
    int8_t _current_app = -1;
    uint8_t _prompt_focus = 1;
    bool _dragging = false;
    bool _suppress_click = false;
    bool _mic_expand = false;
    uint8_t _assistant_wave_step = 0;
    uint32_t _session_start_tick = 0;
    uint8_t _realtime_sentence_index = 0;
    uint8_t _realtime_stage = 0;
    uint8_t _realtime_delay = 0;
    uint16_t _realtime_progress = 0;
    uint8_t _meeting_step = 0;
    uint8_t _meeting_line_index = 0;
    uint8_t _meeting_line_delay = 0;
    uint8_t _meeting_mic_opacity_step = 0;
    uint16_t _meeting_line_progress = 0;
    lv_coord_t _meeting_transcript_target_width = 0;
    uint8_t _music_track_index = 0;
    uint16_t _music_elapsed_seconds = 72;
    bool _music_playing = true;
    uint32_t _camera_last_click_ms = 0;
    uint16_t _camera_record_seconds = 0;
    uint16_t _camera_toast_remaining_ms = 0;
    bool _camera_recording = false;
    NavigationState _navigation_state = {};
    NavigationLightState _nav_rendered_light_state = NavigationLightState::None;
};

#include "LensReactUI.hpp"

#include <cstdio>
#include <cstring>
#include <ctime>

#if defined(lv_timer_get_user_data)
#define LENS_TIMER_USER_DATA(t) lv_timer_get_user_data(t)
#else
#define LENS_TIMER_USER_DATA(t) ((t)->user_data)
#endif

#ifndef SMARTGLASS_ENABLE_SIMULATION_BACKGROUNDS
#define SMARTGLASS_ENABLE_SIMULATION_BACKGROUNDS 1
#endif

LV_FONT_DECLARE(smartglass_font_16_cjk);
LV_FONT_DECLARE(smartglass_font_14_cjk);
LV_FONT_DECLARE(smartglass_font_12_cjk);
LV_FONT_DECLARE(smartglass_font_20_cjk);
LV_FONT_DECLARE(nav_font_72_digits);
LV_IMG_DECLARE(assistant_bot_icon);
LV_IMG_DECLARE(camera_shutter_button);
LV_IMG_DECLARE(nav_bicycle_icon);
LV_IMG_DECLARE(nav_flag_icon);
LV_IMG_DECLARE(nav_bicycle);
LV_IMG_DECLARE(nav_direction_straight);
LV_IMG_DECLARE(nav_direction_left);
LV_IMG_DECLARE(nav_direction_right);
LV_IMG_DECLARE(nav_direction_slight_left);
LV_IMG_DECLARE(nav_direction_slight_right);
LV_IMG_DECLARE(nav_direction_uturn);
LV_IMG_DECLARE(nav_flag);
LV_IMG_DECLARE(nav_traffic_red);
LV_IMG_DECLARE(nav_traffic_green);
LV_IMG_DECLARE(meeting_microphone_icon);
#if SMARTGLASS_ENABLE_SIMULATION_BACKGROUNDS
LV_IMG_DECLARE(meeting_background);
LV_IMG_DECLARE(camera_lifestyle_background);
LV_IMG_DECLARE(realtime_translate_background);
LV_IMG_DECLARE(navigation_background);
#endif
LV_IMG_DECLARE(session_clock_icon);
LV_IMG_DECLARE(home_notes_icon);
LV_IMG_DECLARE(home_camera_icon);
LV_IMG_DECLARE(home_navigation_icon);
LV_IMG_DECLARE(home_notifications_icon);
LV_IMG_DECLARE(home_music_icon);
LV_IMG_DECLARE(home_prompter_icon);
LV_IMG_DECLARE(home_translate_icon);
LV_IMG_DECLARE(home_bilingual_chat_icon);
LV_IMG_DECLARE(home_meeting_icon);
LV_IMG_DECLARE(home_settings_icon);
LV_IMG_DECLARE(home_assistant_icon);

namespace {
constexpr lv_color_t kBlack = LV_COLOR_MAKE(0x00, 0x00, 0x00);
constexpr lv_color_t kWhite = LV_COLOR_MAKE(0xff, 0xff, 0xff);
constexpr lv_color_t kHomeBackground = LV_COLOR_MAKE(0xff, 0xff, 0xff);
constexpr lv_color_t kHomeShell = LV_COLOR_MAKE(0xf1, 0xf1, 0xf3);
constexpr lv_color_t kHomeChrome = LV_COLOR_MAKE(0x18, 0x18, 0x1b);
constexpr lv_color_t kHomeTextDim = LV_COLOR_MAKE(0x52, 0x52, 0x5a);
constexpr lv_color_t kHomeDotDim = LV_COLOR_MAKE(0x9c, 0x9c, 0xa3);
constexpr lv_color_t kHomeBorder = LV_COLOR_MAKE(0x26, 0x26, 0x2a);
constexpr lv_color_t kSelectedBorder = LV_COLOR_MAKE(0xd4, 0xd4, 0xd8);
constexpr lv_color_t kPanel = LV_COLOR_MAKE(0x18, 0x18, 0x1b);
constexpr lv_color_t kPanelSoft = LV_COLOR_MAKE(0x27, 0x27, 0x2a);
constexpr lv_color_t kText = LV_COLOR_MAKE(0xf4, 0xf4, 0xf5);
constexpr lv_color_t kTextDim = LV_COLOR_MAKE(0xa1, 0xa1, 0xaa);
constexpr lv_color_t kTextFaint = LV_COLOR_MAKE(0x63, 0x63, 0x6b);
constexpr lv_color_t kYellow = LV_COLOR_MAKE(0xfa, 0xcc, 0x15);
constexpr lv_color_t kBlue = LV_COLOR_MAKE(0x3b, 0x82, 0xf6);
constexpr lv_color_t kRed = LV_COLOR_MAKE(0xf8, 0x71, 0x71);
constexpr lv_color_t kGreen = LV_COLOR_MAKE(0x34, 0xd3, 0x99);
constexpr lv_color_t kCyan = LV_COLOR_MAKE(0x22, 0xd3, 0xee);
constexpr lv_color_t kOrange = LV_COLOR_MAKE(0xea, 0x58, 0x0c);
constexpr lv_color_t kPurple = LV_COLOR_MAKE(0xa7, 0x8b, 0xfa);
constexpr lv_color_t kMeetingMicIdle = LV_COLOR_MAKE(0x77, 0x77, 0x7d);
constexpr lv_color_t kMeetingMicActive = LV_COLOR_MAKE(0x8e, 0x5d, 0xe7);
constexpr lv_color_t kHudGreen = LV_COLOR_MAKE(0x31, 0xff, 0x65);
constexpr lv_color_t kHudDot = LV_COLOR_MAKE(0xdc, 0xff, 0xe5);
constexpr uint8_t kBatteryPercent = 88;
constexpr lv_coord_t kStatusBarHeight = 30;
constexpr lv_coord_t kPageBottomSafeArea = 0;
constexpr lv_coord_t kCompactViewMaxHeight = 360;
constexpr lv_coord_t kCompactContentTop = 38;
constexpr uint32_t kAssistantWakeKey = 'a';

struct TileDef {
    const char *id;
    const char *name;
    const lv_img_dsc_t *icon;
};

struct MusicTrack {
    const char *title;
    const char *artist;
    uint16_t duration_seconds;
    lv_color_t cover_start;
    lv_color_t cover_end;
};

const TileDef kApps[] = {
    {"notes", "备忘", &home_notes_icon},
    {"camera", "相机", &home_camera_icon},
    {"nav", "导航", &home_navigation_icon},
    {"notify", "通知", &home_notifications_icon},
    {"music", "音乐", &home_music_icon},
    {"prompter", "提词", &home_prompter_icon},
    {"translate", "翻译", &home_translate_icon},
    {"realtime_translate", "实时翻译", &home_translate_icon},
    {"bilingual_chat", "双语畅聊", &home_bilingual_chat_icon},
    {"meeting", "会议记录", &home_meeting_icon},
    {"settings", "设置", &home_settings_icon},
    {"assistant", "小智", &home_assistant_icon},
};

const MusicTrack kMusicTracks[] = {
    {"Cyberpunk City", "Synthwave Maker", 222, LV_COLOR_MAKE(0xc0, 0x4b, 0xf5), LV_COLOR_MAKE(0x18, 0xb7, 0xd5)},
    {"Night Drive", "Neon Pulse", 196, LV_COLOR_MAKE(0x36, 0x60, 0xf6), LV_COLOR_MAKE(0x34, 0xd3, 0x99)},
    {"Glass Horizon", "Ambient Lab", 248, LV_COLOR_MAKE(0xf4, 0x73, 0xb9), LV_COLOR_MAKE(0xf5, 0x9e, 0x0b)},
};

const char *kPrompterSegments[] = {
    "大家好，欢迎来到新品发布会。",
    "我们的 AR 眼镜采用了全新的设计，",
    " Our AR glasses feature a broader design，",
    " 支持全球超过 100 种语言的实时翻译，",
    " and support translation for over 100 languages. ",
    "准确率高达 98%，",
    "做到真正意义上的无障碍沟通。",
    "感谢大家的关注与支持。",
};

constexpr uint8_t kPrompterSegmentCount = sizeof(kPrompterSegments) / sizeof(kPrompterSegments[0]);

constexpr uint8_t kMusicTrackCount = sizeof(kMusicTracks) / sizeof(kMusicTracks[0]);

const lv_font_t *font_or_default(const lv_font_t *font)
{
    return font ? font : LV_FONT_DEFAULT;
}

void set_plain(lv_obj_t *obj)
{
    lv_obj_remove_style_all(obj);
    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
}

lv_obj_t *label(lv_obj_t *parent, const char *text, const lv_font_t *font, lv_color_t color)
{
    lv_obj_t *obj = lv_label_create(parent);
    lv_label_set_text(obj, text);
    lv_obj_set_style_text_font(obj, font_or_default(font), 0);
    lv_obj_set_style_text_color(obj, color, 0);
    return obj;
}

lv_obj_t *cjk_label(lv_obj_t *parent, const char *text, lv_color_t color)
{
    return label(parent, text, &smartglass_font_16_cjk, color);
}

void format_time(char *buffer, size_t buffer_size, uint16_t seconds)
{
    std::snprintf(buffer, buffer_size, "%u:%02u", static_cast<unsigned>(seconds / 60),
                  static_cast<unsigned>(seconds % 60));
}

uint8_t utf8_char_width(unsigned char value)
{
    if((value & 0x80u) == 0) {
        return 1;
    }
    if((value & 0xe0u) == 0xc0u) {
        return 2;
    }
    if((value & 0xf0u) == 0xe0u) {
        return 3;
    }
    if((value & 0xf8u) == 0xf0u) {
        return 4;
    }
    return 1;
}

uint16_t utf8_char_count(const char *text)
{
    if(text == nullptr) {
        return 0;
    }
    uint16_t count = 0;
    for(size_t i = 0; text[i] != '\0';) {
        i += utf8_char_width(static_cast<unsigned char>(text[i]));
        count++;
    }
    return count;
}

uint32_t prompter_segment_duration(const char *text)
{
    uint16_t cjk_chars = 0;
    uint16_t ascii_words = 0;
    bool in_word = false;
    for(size_t i = 0; text && text[i] != '\0';) {
        const unsigned char value = static_cast<unsigned char>(text[i]);
        if(value < 0x80u) {
            const bool word_char = (value >= '0' && value <= '9') || (value >= 'A' && value <= 'Z') ||
                                   (value >= 'a' && value <= 'z');
            if(word_char && !in_word) {
                ascii_words++;
            }
            in_word = word_char;
            i++;
        } else {
            cjk_chars++;
            in_word = false;
            i += utf8_char_width(value);
        }
    }
    const uint32_t duration = 250u + cjk_chars * 115u + ascii_words * 300u;
    return LV_MIN(2400u, LV_MAX(900u, duration));
}

void copy_utf8_prefix(char *buffer, size_t buffer_size, const char *text, uint16_t char_count)
{
    if(buffer == nullptr || buffer_size == 0) {
        return;
    }
    buffer[0] = '\0';
    if(text == nullptr || char_count == 0) {
        return;
    }
    size_t bytes = 0;
    uint16_t copied = 0;
    while(text[bytes] != '\0' && copied < char_count) {
        const uint8_t width = utf8_char_width(static_cast<unsigned char>(text[bytes]));
        if(bytes + width >= buffer_size) {
            break;
        }
        bytes += width;
        copied++;
    }
    std::memcpy(buffer, text, bytes);
    buffer[bytes] = '\0';
}

void set_label_utf8_prefix(lv_obj_t *label_obj, const char *text, uint16_t char_count)
{
    if(label_obj == nullptr) {
        return;
    }
    char buffer[256];
    copy_utf8_prefix(buffer, sizeof(buffer), text, char_count);
    lv_label_set_text(label_obj, buffer);
}

lv_obj_t *box(lv_obj_t *parent, lv_coord_t w, lv_coord_t h, lv_color_t bg, lv_opa_t opa, lv_coord_t radius)
{
    lv_obj_t *obj = lv_obj_create(parent);
    lv_obj_set_size(obj, w, h);
    lv_obj_set_style_bg_color(obj, bg, 0);
    lv_obj_set_style_bg_opa(obj, opa, 0);
    lv_obj_set_style_border_width(obj, 0, 0);
    lv_obj_set_style_radius(obj, radius, 0);
    lv_obj_set_style_pad_all(obj, 0, 0);
    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
    return obj;
}

lv_obj_t *cover_image(lv_obj_t *parent, const lv_img_dsc_t *source, lv_coord_t target_width,
                      lv_coord_t target_height)
{
    constexpr int32_t source_width = 512;
    constexpr int32_t source_height = 288;
    const int32_t zoom_x = (static_cast<int32_t>(target_width) * 256 + source_width - 1) / source_width;
    const int32_t zoom_y = (static_cast<int32_t>(target_height) * 256 + source_height - 1) / source_height;
    const uint16_t zoom = static_cast<uint16_t>(LV_MAX(zoom_x, zoom_y));

    lv_obj_t *image = lv_img_create(parent);
    lv_img_set_src(image, source);
    lv_img_set_pivot(image, source_width / 2, source_height / 2);
    lv_img_set_zoom(image, zoom);
    lv_obj_center(image);
    lv_obj_clear_flag(image, LV_OBJ_FLAG_CLICKABLE);
    return image;
}

lv_obj_t *line(lv_obj_t *parent, const lv_point_t *points, uint16_t count, lv_color_t color, lv_opa_t opa, lv_coord_t width)
{
    lv_obj_t *obj = lv_line_create(parent);
    lv_line_set_points(obj, points, count);
    lv_obj_set_style_line_color(obj, color, 0);
    lv_obj_set_style_line_opa(obj, opa, 0);
    lv_obj_set_style_line_width(obj, width, 0);
    lv_obj_set_style_line_rounded(obj, true, 0);
    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
    return obj;
}

void add_camera_corner(lv_obj_t *parent, lv_align_t align, bool right, bool bottom, lv_coord_t length, lv_coord_t thickness,
                       lv_coord_t inset, lv_color_t color, lv_opa_t opa)
{
    lv_obj_t *corner = box(parent, length, length, kBlack, LV_OPA_TRANSP, 0);
    lv_obj_align(corner, align, right ? -inset : inset, bottom ? -inset : inset);

    lv_obj_t *h = box(corner, length, thickness, color, opa, thickness / 2);
    lv_obj_set_pos(h, 0, bottom ? (length - thickness) : 0);

    lv_obj_t *v = box(corner, thickness, length, color, opa, thickness / 2);
    lv_obj_set_pos(v, right ? (length - thickness) : 0, 0);
}

void style_button(lv_obj_t *btn, lv_color_t color, lv_opa_t opa)
{
    lv_obj_set_style_bg_color(btn, color, 0);
    lv_obj_set_style_bg_opa(btn, opa, 0);
    lv_obj_set_style_border_width(btn, 0, 0);
    lv_obj_set_style_shadow_width(btn, 0, 0);
    lv_obj_set_style_radius(btn, LV_RADIUS_CIRCLE, 0);
}

void style_no_frame(lv_obj_t *obj)
{
    lv_obj_set_style_bg_opa(obj, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(obj, 0, 0);
    lv_obj_set_style_outline_width(obj, 0, 0);
    lv_obj_set_style_shadow_width(obj, 0, 0);
}

void style_soft_row(lv_obj_t *row)
{
    lv_obj_set_style_bg_color(row, LV_COLOR_MAKE(0x22, 0x23, 0x27), 0);
    lv_obj_set_style_bg_opa(row, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(row, 1, 0);
    lv_obj_set_style_border_color(row, LV_COLOR_MAKE(0x45, 0x46, 0x4d), 0);
    lv_obj_set_style_border_opa(row, LV_OPA_60, 0);
    lv_obj_set_style_shadow_width(row, 12, 0);
    lv_obj_set_style_shadow_color(row, kBlack, 0);
    lv_obj_set_style_shadow_opa(row, LV_OPA_50, 0);
}

void style_soft_action(lv_obj_t *obj, lv_color_t accent)
{
    lv_obj_set_style_bg_color(obj, LV_COLOR_MAKE(0x1b, 0x19, 0x12), 0);
    lv_obj_set_style_bg_opa(obj, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(obj, 1, 0);
    lv_obj_set_style_border_color(obj, accent, 0);
    lv_obj_set_style_border_opa(obj, LV_OPA_40, 0);
    lv_obj_set_style_shadow_width(obj, 10, 0);
    lv_obj_set_style_shadow_color(obj, kBlack, 0);
    lv_obj_set_style_shadow_opa(obj, LV_OPA_30, 0);
}

bool fill_local_time(std::tm *time_info)
{
    if(time_info == nullptr) {
        return false;
    }
    std::time_t now = std::time(nullptr);
#if defined(_WIN32)
    return localtime_s(time_info, &now) == 0;
#else
    return localtime_r(&now, time_info) != nullptr;
#endif
}

const lv_img_dsc_t *navigation_direction_image(NavigationDirection direction)
{
    switch(direction) {
    case NavigationDirection::Left:
        return &nav_direction_left;
    case NavigationDirection::Right:
        return &nav_direction_right;
    case NavigationDirection::SlightLeft:
        return &nav_direction_slight_left;
    case NavigationDirection::SlightRight:
        return &nav_direction_slight_right;
    case NavigationDirection::UTurn:
        return &nav_direction_uturn;
    case NavigationDirection::Straight:
    default:
        return &nav_direction_straight;
    }
}
}

LensReactUI::LensReactUI()
{
}

bool LensReactUI::init(void)
{
    return true;
}

bool LensReactUI::run(void)
{
    if(_root != nullptr) {
        return true;
    }

    lv_disp_t *display = lv_disp_get_default();
    if(display == nullptr) {
        return false;
    }

    _width = lv_disp_get_hor_res(display);
    _height = lv_disp_get_ver_res(display);
    if(_width <= 0 || _height <= 0) {
        return false;
    }
    _selected_index = 2;
    _current_app = -1;
    _prompt_focus = 1;
    _view_width = _width;
    _view_height = _height;
    _item_step = 118;

    _root = lv_obj_create(lv_scr_act());
    set_plain(_root);
    lv_obj_set_size(_root, _width, _height);
    lv_obj_set_pos(_root, 0, 0);
    lv_obj_set_style_bg_color(_root, kBlack, 0);
    lv_obj_set_style_bg_opa(_root, LV_OPA_COVER, 0);
    lv_obj_add_flag(_root, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(_root, onRootPressed, LV_EVENT_PRESSED, this);
    lv_obj_add_event_cb(_root, onRootReleased, LV_EVENT_RELEASED, this);
    lv_obj_add_event_cb(_root, onRootReleased, LV_EVENT_PRESS_LOST, this);
    lv_obj_add_event_cb(_root, onRootKey, LV_EVENT_KEY, this);

    lv_group_t *group = lv_group_get_default();
    if(group) {
        lv_group_add_obj(group, _root);
        lv_group_focus_obj(_root);
    }

    _viewport = lv_obj_create(_root);
    set_plain(_viewport);
    lv_obj_set_size(_viewport, _view_width, _view_height);
    lv_obj_center(_viewport);
    lv_obj_set_style_bg_color(_viewport, kBlack, 0);
    lv_obj_set_style_bg_opa(_viewport, LV_OPA_COVER, 0);
    lv_obj_add_flag(_viewport, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_style_clip_corner(_viewport, true, 0);
    lv_obj_add_event_cb(_viewport, onRootPressed, LV_EVENT_PRESSED, this);
    lv_obj_add_event_cb(_viewport, onRootReleased, LV_EVENT_RELEASED, this);
    lv_obj_add_event_cb(_viewport, onRootReleased, LV_EVENT_PRESS_LOST, this);
    lv_obj_add_event_cb(_viewport, onRootKey, LV_EVENT_KEY, this);

    createHome(_view_width, _view_height);
    createPageHost(_view_width, _view_height);
    createStatusBar(_view_width, _view_height);
    selectIndex(_selected_index, false);
    showHome();
    updateClock();

    _clock_timer = lv_timer_create(onClockTimer, 1000, this);
    _prompt_timer = lv_timer_create(onPromptTimer, 1200, this);
    _mic_timer = lv_timer_create(onMicTimer, 650, this);
    _navigation_timer = lv_timer_create(onNavigationTimer, 1000, this);
    _notification_timer = lv_timer_create(onNotificationTimer, kNotificationTimelineTickMs, this);
    _translate_timer = lv_timer_create(onTranslateTimer, 100, this);
    _realtime_translate_timer = lv_timer_create(onRealtimeTranslateTimer, 80, this);
    _bilingual_timer = lv_timer_create(onBilingualChatTimer, 70, this);
    _meeting_timer = lv_timer_create(onMeetingTimer, 90, this);
    _music_timer = lv_timer_create(onMusicTimer, 1000, this);
    _camera_timer = lv_timer_create(onCameraTimer, 1000, this);
    pausePageTimers();
    if(_mic_timer) {
        lv_timer_pause(_mic_timer);
    }

    return true;
}

bool LensReactUI::back(void)
{
    if(_assistant_overlay) {
        hideAssistantOverlay();
        return true;
    }
    if(_current_app >= 0) {
        showHome();
        return true;
    }
    return true;
}

bool LensReactUI::close(void)
{
    if(_clock_timer) {
        lv_timer_del(_clock_timer);
        _clock_timer = nullptr;
    }
    if(_prompt_timer) {
        lv_timer_del(_prompt_timer);
        _prompt_timer = nullptr;
    }
    if(_mic_timer) {
        lv_timer_del(_mic_timer);
        _mic_timer = nullptr;
    }
    if(_navigation_timer) {
        lv_timer_del(_navigation_timer);
        _navigation_timer = nullptr;
    }
    if(_notification_timer) {
        lv_timer_del(_notification_timer);
        _notification_timer = nullptr;
    }
    if(_translate_timer) {
        lv_timer_del(_translate_timer);
        _translate_timer = nullptr;
    }
    if(_realtime_translate_timer) {
        lv_timer_del(_realtime_translate_timer);
        _realtime_translate_timer = nullptr;
    }
    if(_bilingual_timer) {
        lv_timer_del(_bilingual_timer);
        _bilingual_timer = nullptr;
    }
    if(_meeting_timer) {
        lv_timer_del(_meeting_timer);
        _meeting_timer = nullptr;
    }
    if(_music_timer) {
        lv_timer_del(_music_timer);
        _music_timer = nullptr;
    }
    if(_camera_timer) {
        lv_timer_del(_camera_timer);
        _camera_timer = nullptr;
    }
    clearNotificationBubbles();
    clearTranslateItems();
    clearBilingualChatItems();
    if(_root) {
        lv_obj_del(_root);
        _root = nullptr;
    }
    _viewport = nullptr;
    _home = nullptr;
    _track = nullptr;
    _status_bar = nullptr;
    _dot_row = nullptr;
    _assistant_button = nullptr;
    _page = nullptr;
    _page_content = nullptr;
    _clock_label = nullptr;
    _battery_label = nullptr;
    _battery_fill = nullptr;
    _assistant_overlay = nullptr;
    _assistant_panel = nullptr;
    for(auto &bar : _assistant_wave_bars) {
        bar = nullptr;
    }
    _notification_stack = nullptr;
    _notification_empty_label = nullptr;
    _translate_stack = nullptr;
    _translate_empty = nullptr;
    _session_timer_pill = nullptr;
    _session_elapsed_label = nullptr;
    _session_start_tick = 0;
    _realtime_source_label = nullptr;
    _realtime_translation_label = nullptr;
    _bilingual_stack = nullptr;
    _bilingual_empty = nullptr;
    _meeting_status_pill = nullptr;
    _meeting_status_label = nullptr;
    _meeting_transcript_pill = nullptr;
    _meeting_transcript_content = nullptr;
    _meeting_mic_icon = nullptr;
    _meeting_transcript_label = nullptr;
    for(auto &card : _meeting_cards) {
        card = nullptr;
    }
    _music_cover = nullptr;
    _music_title = nullptr;
    _music_artist = nullptr;
    _music_elapsed_label = nullptr;
    _music_duration_label = nullptr;
    _music_progress_fill = nullptr;
    _music_play_icon = nullptr;
    _camera_recording_pill = nullptr;
    _camera_recording_label = nullptr;
    _camera_toast = nullptr;
    _camera_toast_label = nullptr;
    return true;
}

void LensReactUI::createHome(lv_coord_t width, lv_coord_t height)
{
    _home = lv_obj_create(_viewport);
    set_plain(_home);
    lv_obj_set_size(_home, width, height);
    lv_obj_set_style_bg_color(_home, kHomeBackground, 0);
    lv_obj_set_style_bg_opa(_home, LV_OPA_COVER, 0);

    lv_obj_t *rail = lv_obj_create(_home);
    set_plain(rail);
    lv_obj_set_size(rail, LV_MIN((lv_coord_t)640, width), 132);
    lv_obj_align(rail, LV_ALIGN_BOTTOM_MID, 0, -18);
    lv_obj_set_style_bg_opa(rail, LV_OPA_TRANSP, 0);

    _track = lv_obj_create(rail);
    set_plain(_track);
    lv_obj_set_size(_track, (kAppCount - 1) * _item_step + 104, 132);
    lv_obj_set_y(_track, 0);
    lv_obj_set_style_bg_opa(_track, LV_OPA_TRANSP, 0);

    for(uint8_t i = 0; i < kAppCount; ++i) {
        lv_obj_t *tile = lv_obj_create(_track);
        _tiles[i] = tile;
        set_plain(tile);
        lv_obj_set_size(tile, 104, 116);
        lv_obj_set_pos(tile, i * _item_step, 0);
        lv_obj_set_style_bg_opa(tile, LV_OPA_TRANSP, 0);
        lv_obj_add_flag(tile, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_event_cb(tile, onTileClicked, LV_EVENT_CLICKED, this);
        lv_obj_set_user_data(tile, reinterpret_cast<void *>(static_cast<uintptr_t>(i)));

        lv_obj_t *shell = box(tile, 88, 88, kPanelSoft, LV_OPA_TRANSP, 14);
        lv_obj_align(shell, LV_ALIGN_TOP_MID, 0, 0);
        lv_obj_set_style_border_width(shell, 2, 0);
        lv_obj_clear_flag(shell, LV_OBJ_FLAG_CLICKABLE);

        lv_obj_t *icon_bg = box(shell, 50, 50, kPanel, LV_OPA_COVER, 8);
        lv_obj_center(icon_bg);
        lv_obj_clear_flag(icon_bg, LV_OBJ_FLAG_CLICKABLE);

        _tile_icons[i] = lv_img_create(icon_bg);
        lv_img_set_src(_tile_icons[i], kApps[i].icon);
        lv_obj_center(_tile_icons[i]);
        lv_obj_clear_flag(_tile_icons[i], LV_OBJ_FLAG_CLICKABLE);

        _tile_names[i] = cjk_label(tile, kApps[i].name, kText);
        lv_obj_set_width(_tile_names[i], 104);
        lv_label_set_long_mode(_tile_names[i], LV_LABEL_LONG_DOT);
        lv_obj_set_style_text_align(_tile_names[i], LV_TEXT_ALIGN_CENTER, 0);
        lv_obj_align(_tile_names[i], LV_ALIGN_TOP_MID, 0, 94);
        lv_obj_clear_flag(_tile_names[i], LV_OBJ_FLAG_CLICKABLE);
    }

    _dot_row = lv_obj_create(_home);
    set_plain(_dot_row);
    lv_obj_set_size(_dot_row, kAppCount * 13 + 16, 10);
    lv_obj_align(_dot_row, LV_ALIGN_BOTTOM_MID, 0, -4);
    lv_obj_set_style_bg_opa(_dot_row, LV_OPA_TRANSP, 0);

    for(uint8_t i = 0; i < kAppCount; ++i) {
        _dots[i] = box(_dot_row, 4, 4, kTextDim, LV_OPA_COVER, LV_RADIUS_CIRCLE);
        lv_obj_set_pos(_dots[i], 6 + i * 13, 3);
    }
}

void LensReactUI::createStatusBar(lv_coord_t width, lv_coord_t height)
{
    _status_bar = lv_obj_create(_viewport);
    set_plain(_status_bar);
    lv_obj_set_size(_status_bar, width, kStatusBarHeight);
    lv_obj_align(_status_bar, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_set_style_bg_opa(_status_bar, LV_OPA_TRANSP, 0);

    _clock_label = label(_status_bar, "9:41", &lv_font_montserrat_16, kText);
    lv_obj_align(_clock_label, LV_ALIGN_TOP_LEFT, 18, 6);

    constexpr lv_coord_t battery_slot_width = 28;
    constexpr lv_coord_t battery_slot_height = 9;
    lv_obj_t *battery = box(_status_bar, 36, 15, kBlack, LV_OPA_COVER, 4);
    lv_obj_align(battery, LV_ALIGN_TOP_RIGHT, -66, 7);
    lv_obj_set_style_border_width(battery, 2, 0);
    lv_obj_set_style_border_color(battery, kWhite, 0);

    lv_obj_t *battery_slot = box(battery, battery_slot_width, battery_slot_height, kBlack, LV_OPA_COVER, 2);
    lv_obj_align(battery_slot, LV_ALIGN_LEFT_MID, 3, 0);
    const lv_coord_t fill_width = (lv_coord_t)((battery_slot_width * kBatteryPercent) / 100);
    _battery_fill = box(battery_slot, fill_width, battery_slot_height, kWhite, LV_OPA_COVER, 2);
    lv_obj_align(_battery_fill, LV_ALIGN_LEFT_MID, 0, 0);

    lv_obj_t *battery_cap = box(_status_bar, 3, 7, kWhite, LV_OPA_COVER, 2);
    lv_obj_align_to(battery_cap, battery, LV_ALIGN_OUT_RIGHT_MID, 1, 0);

    char battery_text[8];
    std::snprintf(battery_text, sizeof(battery_text), "%u%%", kBatteryPercent);
    _battery_label = label(_status_bar, battery_text, &lv_font_montserrat_16, kText);
    lv_obj_align(_battery_label, LV_ALIGN_TOP_RIGHT, -16, 6);

    _assistant_button = box(_status_bar, 42, 20, LV_COLOR_MAKE(0x24, 0x1b, 0x43), LV_OPA_COVER, 10);
    lv_obj_align(_assistant_button, LV_ALIGN_TOP_RIGHT, -108, 5);
    lv_obj_add_flag(_assistant_button, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(_assistant_button, onAssistantClicked, LV_EVENT_CLICKED, this);
    lv_obj_t *assistant_text = cjk_label(_assistant_button, "小智", kPurple);
    lv_obj_center(assistant_text);

    LV_UNUSED(height);
}

void LensReactUI::createPageHost(lv_coord_t width, lv_coord_t height)
{
    _page = lv_obj_create(_viewport);
    set_plain(_page);
    lv_obj_set_size(_page, width, height);
    lv_obj_set_style_bg_color(_page, kBlack, 0);
    lv_obj_set_style_bg_opa(_page, LV_OPA_COVER, 0);
    lv_obj_add_flag(_page, LV_OBJ_FLAG_HIDDEN);

    _page_content = lv_obj_create(_page);
    set_plain(_page_content);
    lv_obj_set_size(_page_content, width, height - kPageBottomSafeArea);
    lv_obj_align(_page_content, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_set_style_bg_opa(_page_content, LV_OPA_TRANSP, 0);
    lv_obj_move_background(_page_content);
}

void LensReactUI::selectIndex(uint8_t index, bool animate)
{
    if(index >= kAppCount) {
        return;
    }
    _selected_index = index;
    const lv_coord_t rail_width = _track ? lv_obj_get_width(lv_obj_get_parent(_track)) : _view_width;
    const lv_coord_t content_width = (kAppCount - 1) * _item_step + 104;
    lv_coord_t target_x = (rail_width / 2) - 52 - (lv_coord_t)(_selected_index * _item_step);
    const lv_coord_t min_x = rail_width - content_width - 12;
    if(target_x > 12) {
        target_x = 12;
    } else if(target_x < min_x) {
        target_x = min_x;
    }

    if(animate) {
        lv_anim_t anim;
        lv_anim_init(&anim);
        lv_anim_set_var(&anim, _track);
        lv_anim_set_values(&anim, lv_obj_get_x(_track), target_x);
        lv_anim_set_time(&anim, 180);
        lv_anim_set_path_cb(&anim, lv_anim_path_ease_out);
        lv_anim_set_exec_cb(&anim, onAnimX);
        lv_anim_start(&anim);
    } else {
        lv_obj_set_x(_track, target_x);
    }

    updateTileStyles();
    updateDots();
}

void LensReactUI::updateTileStyles(void)
{
    for(uint8_t i = 0; i < kAppCount; ++i) {
        lv_obj_t *tile = _tiles[i];
        if(tile == nullptr) {
            continue;
        }
        lv_obj_t *shell = lv_obj_get_child(tile, 0);
        lv_obj_t *icon_bg = shell ? lv_obj_get_child(shell, 0) : nullptr;
        const bool selected = i == _selected_index;
        const bool on_home = _current_app < 0;
        lv_color_t border_color = on_home ? kHomeBorder : kBlack;
        if(selected) {
            border_color = on_home ? kHomeChrome : kSelectedBorder;
        }
        lv_obj_set_size(shell, selected ? 88 : 64, selected ? 88 : 64);
        lv_obj_align(shell, LV_ALIGN_TOP_MID, 0, selected ? 0 : 10);
        lv_obj_set_style_border_color(shell, border_color, 0);
        lv_obj_set_style_border_opa(shell, selected ? LV_OPA_COVER : LV_OPA_TRANSP, 0);
        lv_obj_set_style_bg_color(shell, on_home ? kHomeShell : kPanelSoft, 0);
        lv_obj_set_style_bg_opa(shell, selected ? LV_OPA_COVER : LV_OPA_TRANSP, 0);
        lv_obj_set_style_opa(tile, LV_OPA_COVER, 0);
        lv_obj_set_style_transform_zoom(tile, 256, 0);
        if(icon_bg) {
            lv_obj_set_size(icon_bg, selected ? 58 : 50, selected ? 58 : 50);
            lv_obj_center(icon_bg);
            lv_obj_set_style_radius(icon_bg, selected ? 10 : 8, 0);
            lv_obj_set_style_bg_color(icon_bg, on_home ? kHomeChrome : kPanel, 0);
        }
        if(_tile_names[i]) {
            lv_obj_set_style_opa(_tile_names[i], LV_OPA_COVER, 0);
            const lv_color_t name_color = on_home ? (selected ? kBlack : kHomeTextDim) : (selected ? kText : kTextDim);
            lv_obj_set_style_text_color(_tile_names[i], name_color, 0);
        }
    }
}

void LensReactUI::updateDots(void)
{
    for(uint8_t i = 0; i < kAppCount; ++i) {
        if(_dots[i] == nullptr) {
            continue;
        }
        const bool selected = i == _selected_index;
        lv_obj_set_size(_dots[i], selected ? 14 : 4, 4);
        lv_obj_set_x(_dots[i], selected ? (2 + i * 13) : (6 + i * 13));
        if(_current_app < 0) {
            lv_obj_set_style_bg_color(_dots[i], selected ? kBlack : kHomeDotDim, 0);
        } else {
            lv_obj_set_style_bg_color(_dots[i], selected ? kText : kTextDim, 0);
        }
    }
}

void LensReactUI::showHome(void)
{
    hideAssistantOverlay();
    pausePageTimers();
    _current_app = -1;
    if(_home) {
        lv_obj_clear_flag(_home, LV_OBJ_FLAG_HIDDEN);
    }
    if(_page) {
        lv_obj_add_flag(_page, LV_OBJ_FLAG_HIDDEN);
    }
    if(_battery_label) {
        lv_obj_clear_flag(_battery_label, LV_OBJ_FLAG_HIDDEN);
    }
    if(_dot_row) {
        lv_obj_clear_flag(_dot_row, LV_OBJ_FLAG_HIDDEN);
    }
    if(_assistant_button) {
        lv_obj_clear_flag(_assistant_button, LV_OBJ_FLAG_HIDDEN);
    }
    if(_status_bar) {
        lv_obj_set_style_text_color(_clock_label, kBlack, 0);
        lv_obj_set_style_text_color(_battery_label, kBlack, 0);
        lv_obj_set_style_bg_color(_battery_fill, kBlack, 0);
        uint32_t child_count = lv_obj_get_child_cnt(_status_bar);
        for(uint32_t i = 0; i < child_count; ++i) {
            lv_obj_t *child = lv_obj_get_child(_status_bar, i);
            if(child == _assistant_button) {
                continue;
            }
            lv_obj_set_style_border_color(child, kBlack, 0);
            if(lv_obj_get_width(child) <= 4 && lv_obj_get_height(child) <= 10) {
                lv_obj_set_style_bg_color(child, kBlack, 0);
            }
        }
        lv_obj_move_foreground(_status_bar);
    }
    updateTileStyles();
    updateDots();
    lv_group_t *group = lv_group_get_default();
    if(group && _root) {
        lv_group_focus_obj(_root);
    }
}

void LensReactUI::showApp(uint8_t index)
{
    if(index >= kAppCount) {
        return;
    }
    if(index == kAssistantAppIndex) {
        showAssistantOverlay();
        return;
    }
    hideAssistantOverlay();
    _current_app = static_cast<int8_t>(index);
    if(_home) {
        lv_obj_add_flag(_home, LV_OBJ_FLAG_HIDDEN);
    }
    if(_page) {
        lv_obj_clear_flag(_page, LV_OBJ_FLAG_HIDDEN);
        lv_obj_move_foreground(_page);
    }
    if(_battery_label) {
        lv_obj_clear_flag(_battery_label, LV_OBJ_FLAG_HIDDEN);
    }
    if(_dot_row) {
        lv_obj_add_flag(_dot_row, LV_OBJ_FLAG_HIDDEN);
    }
    if(_assistant_button) {
        lv_obj_add_flag(_assistant_button, LV_OBJ_FLAG_HIDDEN);
    }
    if(_status_bar) {
        lv_obj_set_style_text_color(_clock_label, kText, 0);
        lv_obj_set_style_text_color(_battery_label, kText, 0);
        lv_obj_set_style_bg_color(_battery_fill, kWhite, 0);
        uint32_t child_count = lv_obj_get_child_cnt(_status_bar);
        for(uint32_t i = 0; i < child_count; ++i) {
            lv_obj_t *child = lv_obj_get_child(_status_bar, i);
            if(child == _assistant_button) {
                continue;
            }
            lv_obj_set_style_border_color(child, kWhite, 0);
            if(lv_obj_get_width(child) <= 4 && lv_obj_get_height(child) <= 10) {
                lv_obj_set_style_bg_color(child, kWhite, 0);
            }
        }
    }
    rebuildPage();
    resumePageTimer(_current_app);
    if(_status_bar) {
        lv_obj_move_foreground(_status_bar);
    }
    lv_group_t *group = lv_group_get_default();
    if(group && _root) {
        lv_group_focus_obj(_root);
    }
}

void LensReactUI::rebuildPage(void)
{
    clearPageContent();
    switch(_current_app) {
    case 0:
        createNotesPage();
        break;
    case 1:
        createCameraPage();
        break;
    case 2:
        createNavigationPage();
        break;
    case 3:
        createNotificationPage();
        break;
    case 4:
        createMusicPage();
        break;
    case 5:
        createPrompterPage();
        break;
    case 6:
        createTranslatePage();
        break;
    case 7:
        createRealtimeTranslatePage();
        break;
    case 8:
        createBilingualChatPage();
        break;
    case 9:
        createMeetingPage();
        break;
    case 10:
        createSettingsPage();
        break;
    case 11:
        createAssistantPage();
        break;
    default:
        break;
    }
}

void LensReactUI::clearPageContent(void)
{
    pausePageTimers();
    clearNotificationBubbles();
    clearTranslateItems();
    clearBilingualChatItems();
    if(_page_content) {
        lv_obj_clean(_page_content);
    }
    _prompt_text_label = nullptr;
    _prompt_cursor = nullptr;
    _session_timer_pill = nullptr;
    _session_elapsed_label = nullptr;
    _session_start_tick = 0;
    _mic_ring = nullptr;
    _notification_stack = nullptr;
    _notification_empty_label = nullptr;
    _translate_stack = nullptr;
    _translate_empty = nullptr;
    _realtime_source_label = nullptr;
    _realtime_translation_label = nullptr;
    _bilingual_stack = nullptr;
    _bilingual_empty = nullptr;
    _meeting_status_pill = nullptr;
    _meeting_status_label = nullptr;
    _meeting_transcript_pill = nullptr;
    _meeting_transcript_content = nullptr;
    _meeting_mic_icon = nullptr;
    _meeting_transcript_label = nullptr;
    for(auto &card : _meeting_cards) {
        card = nullptr;
    }
    _music_cover = nullptr;
    _music_title = nullptr;
    _music_artist = nullptr;
    _music_elapsed_label = nullptr;
    _music_duration_label = nullptr;
    _music_progress_fill = nullptr;
    _music_play_icon = nullptr;
    _camera_recording_pill = nullptr;
    _camera_recording_label = nullptr;
    _camera_toast = nullptr;
    _camera_toast_label = nullptr;
    _camera_recording = false;
    _camera_toast_remaining_ms = 0;
    _nav_direction_img = nullptr;
    _nav_distance_label = nullptr;
    _nav_speed_label = nullptr;
    _nav_baseline = nullptr;
    _nav_start_dot = nullptr;
    _nav_end_dot = nullptr;
    _nav_traffic_card = nullptr;
    _nav_traffic_img = nullptr;
    _nav_traffic_title = nullptr;
    _nav_traffic_countdown = nullptr;
    _nav_rendered_light_state = NavigationLightState::None;
}

void LensReactUI::createCameraPage(void)
{
    const bool compact = _view_height <= kCompactViewMaxHeight;
    const lv_coord_t inset = compact ? 18 : 28;
    const lv_coord_t frame_width = _view_width - inset * 2;
    const lv_coord_t frame_height = _view_height - (compact ? 54 : 76);
    lv_obj_t *frame = box(_page_content, frame_width, frame_height, LV_COLOR_MAKE(0x12, 0x13, 0x12), LV_OPA_COVER,
                          compact ? 20 : 26);
    lv_obj_align(frame, LV_ALIGN_TOP_MID, 0, compact ? 34 : 44);
    lv_obj_add_flag(frame, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(frame, onCameraGesture, LV_EVENT_SHORT_CLICKED, this);
    lv_obj_add_event_cb(frame, onCameraGesture, LV_EVENT_LONG_PRESSED, this);
    lv_obj_add_event_cb(frame, onCameraGesture, LV_EVENT_RELEASED, this);

#if SMARTGLASS_ENABLE_SIMULATION_BACKGROUNDS
    cover_image(frame, &camera_lifestyle_background, frame_width, frame_height);
    lv_obj_t *camera_shade = box(frame, frame_width, frame_height, kBlack, LV_OPA_20, compact ? 20 : 26);
    lv_obj_center(camera_shade);
    lv_obj_clear_flag(camera_shade, LV_OBJ_FLAG_CLICKABLE);
#else
    lv_obj_set_style_bg_color(frame, LV_COLOR_MAKE(0x11, 0x12, 0x12), 0);
#endif

    lv_obj_t *focus = box(frame, compact ? 58 : 72, compact ? 58 : 72, kBlack, LV_OPA_TRANSP, 0);
    lv_obj_align(focus, LV_ALIGN_CENTER, 0, compact ? 2 : 4);
    add_camera_corner(focus, LV_ALIGN_TOP_LEFT, false, false, compact ? 18 : 22, 3, 0, kText, LV_OPA_80);
    add_camera_corner(focus, LV_ALIGN_TOP_RIGHT, true, false, compact ? 18 : 22, 3, 0, kText, LV_OPA_80);
    add_camera_corner(focus, LV_ALIGN_BOTTOM_LEFT, false, true, compact ? 18 : 22, 3, 0, kText, LV_OPA_80);
    add_camera_corner(focus, LV_ALIGN_BOTTOM_RIGHT, true, true, compact ? 18 : 22, 3, 0, kText, LV_OPA_80);
    lv_obj_t *focus_dot = box(focus, compact ? 16 : 18, compact ? 16 : 18, kText, LV_OPA_30, LV_RADIUS_CIRCLE);
    lv_obj_center(focus_dot);
    lv_obj_set_style_border_width(focus_dot, 3, 0);
    lv_obj_set_style_border_color(focus_dot, kText, 0);
    lv_obj_set_style_border_opa(focus_dot, LV_OPA_80, 0);

    lv_obj_t *left_info = label(frame, "ISO 400\n1/60s", &lv_font_montserrat_12, LV_COLOR_MAKE(0xdb, 0xdc, 0xd8));
    lv_obj_align(left_info, LV_ALIGN_BOTTOM_LEFT, compact ? 22 : 24, compact ? -36 : -40);
    lv_obj_set_style_text_line_space(left_info, 8, 0);

    lv_obj_t *right_info = label(frame, "f/1.8\n4K 60FPS", &lv_font_montserrat_12, LV_COLOR_MAKE(0xdb, 0xdc, 0xd8));
    lv_obj_align(right_info, LV_ALIGN_BOTTOM_RIGHT, compact ? -22 : -24, compact ? -36 : -40);
    lv_obj_set_style_text_align(right_info, LV_TEXT_ALIGN_RIGHT, 0);
    lv_obj_set_style_text_line_space(right_info, 8, 0);

    lv_obj_t *shutter = lv_img_create(frame);
    lv_img_set_src(shutter, &camera_shutter_button);
    style_no_frame(shutter);
    lv_obj_align(shutter, LV_ALIGN_BOTTOM_MID, 0, compact ? -28 : -34);
    lv_obj_add_flag(shutter, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(shutter, onCameraGesture, LV_EVENT_SHORT_CLICKED, this);
    lv_obj_add_event_cb(shutter, onCameraGesture, LV_EVENT_LONG_PRESSED, this);
    lv_obj_add_event_cb(shutter, onCameraGesture, LV_EVENT_RELEASED, this);

    lv_obj_t *hint = cjk_label(frame, "双击拍照 · 长按录像", LV_COLOR_MAKE(0xdb, 0xdc, 0xd8));
    lv_obj_set_width(hint, frame_width - 140);
    lv_label_set_long_mode(hint, LV_LABEL_LONG_DOT);
    lv_obj_set_style_text_align(hint, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_color(hint, LV_COLOR_MAKE(0xdb, 0xdc, 0xd8), 0);
    lv_obj_align(hint, LV_ALIGN_BOTTOM_MID, 0, compact ? -4 : -8);

    _camera_recording_pill = box(frame, compact ? 116 : 128, 34, kBlack, LV_OPA_70, 17);
    lv_obj_align(_camera_recording_pill, LV_ALIGN_TOP_MID, 0, compact ? 30 : 46);
    lv_obj_t *rec_dot = box(_camera_recording_pill, 8, 8, kRed, LV_OPA_COVER, LV_RADIUS_CIRCLE);
    lv_obj_align(rec_dot, LV_ALIGN_LEFT_MID, 16, 0);
    _camera_recording_label = label(_camera_recording_pill, "00:00:00", &lv_font_montserrat_14, kText);
    lv_obj_align(_camera_recording_label, LV_ALIGN_LEFT_MID, 32, 0);
    lv_obj_add_flag(_camera_recording_pill, LV_OBJ_FLAG_HIDDEN);

    _camera_toast = box(frame, compact ? 180 : 220, 50, kBlack, LV_OPA_70, 18);
    lv_obj_align(_camera_toast, LV_ALIGN_CENTER, 0, compact ? -72 : -96);
    lv_obj_set_style_border_width(_camera_toast, 1, 0);
    lv_obj_set_style_border_color(_camera_toast, kWhite, 0);
    lv_obj_set_style_border_opa(_camera_toast, LV_OPA_20, 0);
    _camera_toast_label = cjk_label(_camera_toast, "照片已保存", kText);
    lv_obj_center(_camera_toast_label);
    lv_obj_add_flag(_camera_toast, LV_OBJ_FLAG_HIDDEN);

    updateCameraPage();
}

void LensReactUI::updateCameraPage(void)
{
    if(_camera_recording_pill == nullptr || _camera_recording_label == nullptr) {
        return;
    }
    if(_camera_recording) {
        lv_obj_clear_flag(_camera_recording_pill, LV_OBJ_FLAG_HIDDEN);
        char duration[16];
        std::snprintf(duration, sizeof(duration), "00:%02u:%02u",
                      static_cast<unsigned>(_camera_record_seconds / 60),
                      static_cast<unsigned>(_camera_record_seconds % 60));
        lv_label_set_text(_camera_recording_label, duration);
    } else {
        lv_obj_add_flag(_camera_recording_pill, LV_OBJ_FLAG_HIDDEN);
    }
}

void LensReactUI::captureCameraPhoto(void)
{
    showCameraToast("照片已保存");
}

void LensReactUI::startCameraRecording(void)
{
    if(_camera_recording) {
        return;
    }
    _camera_recording = true;
    _camera_record_seconds = 0;
    updateCameraPage();
}

void LensReactUI::finishCameraRecording(void)
{
    if(!_camera_recording) {
        return;
    }
    char toast[48];
    std::snprintf(toast, sizeof(toast), "录像已保存 00:%02u:%02u",
                  static_cast<unsigned>(_camera_record_seconds / 60),
                  static_cast<unsigned>(_camera_record_seconds % 60));
    _camera_recording = false;
    updateCameraPage();
    showCameraToast(toast);
}

void LensReactUI::showCameraToast(const char *text)
{
    if(_camera_toast == nullptr || _camera_toast_label == nullptr) {
        return;
    }
    lv_label_set_text(_camera_toast_label, text);
    lv_anim_del(_camera_toast, onAnimOpa);
    lv_obj_clear_flag(_camera_toast, LV_OBJ_FLAG_HIDDEN);
    lv_obj_set_style_opa(_camera_toast, LV_OPA_TRANSP, 0);
    lv_obj_move_foreground(_camera_toast);
    lv_anim_t fade;
    lv_anim_init(&fade);
    lv_anim_set_var(&fade, _camera_toast);
    lv_anim_set_values(&fade, LV_OPA_TRANSP, LV_OPA_COVER);
    lv_anim_set_time(&fade, 180);
    lv_anim_set_path_cb(&fade, lv_anim_path_ease_out);
    lv_anim_set_exec_cb(&fade, onAnimOpa);
    lv_anim_start(&fade);
    _camera_toast_remaining_ms = 3000;
}

void LensReactUI::createNotesPage(void)
{
    if(_view_height <= kCompactViewMaxHeight) {
        lv_obj_t *title = cjk_label(_page_content, "备忘", kText);
        lv_obj_align(title, LV_ALIGN_TOP_LEFT, 24, kCompactContentTop);
        lv_obj_t *plus = box(_page_content, 30, 30, LV_COLOR_MAKE(0x1b, 0x19, 0x12), LV_OPA_COVER, 8);
        lv_obj_align(plus, LV_ALIGN_TOP_RIGHT, -24, kCompactContentTop - 6);
        style_soft_action(plus, kYellow);
        lv_obj_t *plus_icon = label(plus, LV_SYMBOL_PLUS, &lv_font_montserrat_16, kYellow);
        lv_obj_center(plus_icon);

        const char *items[] = {"Team sync at 3 PM", "Buy coffee beans", "UI interaction notes"};
        for(uint8_t i = 0; i < 3; ++i) {
            lv_obj_t *row = box(_page_content, _view_width - 48, 48, LV_COLOR_MAKE(0x22, 0x23, 0x27), LV_OPA_COVER, 10);
            lv_obj_align(row, LV_ALIGN_TOP_MID, 0, 70 + i * 56);
            style_soft_row(row);
            lv_obj_t *text = label(row, items[i], &lv_font_montserrat_14, kText);
            lv_obj_align(text, LV_ALIGN_LEFT_MID, 16, 0);
            lv_obj_t *time = label(row, i == 0 ? "12:30" : (i == 1 ? "Yesterday" : "Monday"),
                                   &lv_font_montserrat_12, kTextFaint);
            lv_obj_align(time, LV_ALIGN_RIGHT_MID, -14, 0);
        }
        return;
    }

    lv_obj_t *title = cjk_label(_page_content, "备忘", kText);
    lv_obj_align(title, LV_ALIGN_TOP_LEFT, 50, 96);

    lv_obj_t *plus = box(_page_content, 48, 48, LV_COLOR_MAKE(0x1b, 0x19, 0x12), LV_OPA_COVER, 14);
    lv_obj_align(plus, LV_ALIGN_TOP_RIGHT, -58, 92);
    lv_obj_add_flag(plus, LV_OBJ_FLAG_CLICKABLE);
    style_soft_action(plus, kYellow);
    lv_obj_t *plus_icon = label(plus, LV_SYMBOL_PLUS, &lv_font_montserrat_24, kYellow);
    lv_obj_center(plus_icon);

    struct Note {
        const char *title;
        const char *time;
    };
    const Note notes[] = {
        {"Team sync at 3 PM", "12:30"},
        {"Buy coffee beans and milk", "Yesterday"},
        {"AR glasses UI interaction notes...", "Monday"},
    };

    lv_obj_t *list = lv_obj_create(_page_content);
    lv_obj_set_size(list, _width - 96, _height - 210);
    lv_obj_align(list, LV_ALIGN_TOP_MID, 0, 166);
    lv_obj_set_style_bg_opa(list, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(list, 0, 0);
    lv_obj_set_style_pad_all(list, 0, 0);
    lv_obj_set_style_pad_row(list, 14, 0);
    lv_obj_set_scroll_dir(list, LV_DIR_VER);
    lv_obj_set_scrollbar_mode(list, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_flex_flow(list, LV_FLEX_FLOW_COLUMN);

    for(const auto &note : notes) {
        lv_obj_t *row = box(list, _width - 110, 76, LV_COLOR_MAKE(0x22, 0x23, 0x27), LV_OPA_COVER, 16);
        style_soft_row(row);
        lv_obj_t *note_title = label(row, note.title, &lv_font_montserrat_16, kText);
        lv_obj_align(note_title, LV_ALIGN_LEFT_MID, 22, 0);
        lv_obj_t *time_pill = box(row, 86, 28, LV_COLOR_MAKE(0x30, 0x31, 0x36), LV_OPA_COVER, 14);
        lv_obj_align(time_pill, LV_ALIGN_RIGHT_MID, -18, 0);
        lv_obj_t *time = label(time_pill, note.time, &lv_font_montserrat_14, kTextFaint);
        lv_obj_center(time);
    }
}

void LensReactUI::createNavigationPage(void)
{
    const lv_coord_t content_height = lv_obj_get_height(_page_content);
    const lv_coord_t baseline_y = content_height - 28;
    const lv_coord_t hud_bottom_y = baseline_y - 8;
    const auto align_hud_bottom = [this, hud_bottom_y](lv_obj_t *obj, lv_coord_t x) {
        lv_obj_update_layout(_page_content);
        lv_obj_set_pos(obj, x, hud_bottom_y - lv_obj_get_height(obj));
    };

#if SMARTGLASS_ENABLE_SIMULATION_BACKGROUNDS
    cover_image(_page_content, &navigation_background, _view_width, content_height);
    lv_obj_t *navigation_shade = box(_page_content, _view_width, content_height, kBlack, LV_OPA_30, 0);
    lv_obj_center(navigation_shade);
    lv_obj_clear_flag(navigation_shade, LV_OBJ_FLAG_CLICKABLE);
#else
    lv_obj_t *navigation_backdrop = box(_page_content, _view_width, content_height, kBlack, LV_OPA_COVER, 0);
    lv_obj_center(navigation_backdrop);
    lv_obj_clear_flag(navigation_backdrop, LV_OBJ_FLAG_CLICKABLE);
#endif

    _nav_baseline = box(_page_content, _view_width - 112, 3, kHudGreen, LV_OPA_80, 2);
    lv_obj_set_pos(_nav_baseline, 56, baseline_y);
    _nav_start_dot = box(_page_content, 10, 10, LV_COLOR_MAKE(0xdc, 0xff, 0xe5), LV_OPA_COVER, LV_RADIUS_CIRCLE);
    lv_obj_set_pos(_nav_start_dot, 51, baseline_y - 4);
    _nav_end_dot = box(_page_content, 10, 10, LV_COLOR_MAKE(0xdc, 0xff, 0xe5), LV_OPA_COVER, LV_RADIUS_CIRCLE);
    lv_obj_set_pos(_nav_end_dot, _view_width - 61, baseline_y - 4);

    _nav_direction_img = lv_img_create(_page_content);
    lv_img_set_src(_nav_direction_img, &nav_direction_straight);
    style_no_frame(_nav_direction_img);
    align_hud_bottom(_nav_direction_img, 64);

    _nav_distance_label = label(_page_content, "350", &nav_font_72_digits, kHudGreen);
    align_hud_bottom(_nav_distance_label, 150);

    lv_obj_t *distance_unit = label(_page_content, "m", &lv_font_montserrat_28, kHudGreen);
    align_hud_bottom(distance_unit, 300);

    lv_obj_t *separator_left = label(_page_content, "/", &lv_font_montserrat_38, LV_COLOR_MAKE(0x14, 0x74, 0x43));
    align_hud_bottom(separator_left, 548);

    lv_obj_t *bike = lv_img_create(_page_content);
    lv_img_set_src(bike, &nav_bicycle);
    style_no_frame(bike);
    align_hud_bottom(bike, 606);

    _nav_speed_label = label(_page_content, "35", &lv_font_montserrat_38, kHudGreen);
    align_hud_bottom(_nav_speed_label, 664);

    lv_obj_t *speed_unit = label(_page_content, "km/h", &lv_font_montserrat_16, kHudGreen);
    align_hud_bottom(speed_unit, 721);

    lv_obj_t *separator_right = label(_page_content, "/", &lv_font_montserrat_38, LV_COLOR_MAKE(0x14, 0x74, 0x43));
    align_hud_bottom(separator_right, 775);

    lv_obj_t *remain = box(_page_content, 126, 24, kBlack, LV_OPA_TRANSP, 0);
    lv_obj_t *remain_prefix = cjk_label(remain, "剩余：", LV_COLOR_MAKE(0x86, 0xef, 0xac));
    lv_obj_set_pos(remain_prefix, 0, 3);
    lv_obj_t *remain_value = label(remain, "2.8", &lv_font_montserrat_16, LV_COLOR_MAKE(0x86, 0xef, 0xac));
    lv_obj_set_pos(remain_value, 56, 4);
    lv_obj_t *remain_unit = cjk_label(remain, "公里", LV_COLOR_MAKE(0x86, 0xef, 0xac));
    lv_obj_set_pos(remain_unit, 86, 3);
    align_hud_bottom(remain, 824);

    lv_obj_t *flag = lv_img_create(_page_content);
    lv_img_set_src(flag, &nav_flag);
    style_no_frame(flag);
    align_hud_bottom(flag, 958);

    _nav_traffic_card = box(_page_content, 228, 92, LV_COLOR_MAKE(0x08, 0x08, 0x09), LV_OPA_90, 16);
    lv_obj_align(_nav_traffic_card, LV_ALIGN_TOP_MID, 0, 42);
    lv_obj_set_style_border_width(_nav_traffic_card, 1, 0);
    lv_obj_set_style_border_color(_nav_traffic_card, kRed, 0);
    lv_obj_set_style_border_opa(_nav_traffic_card, LV_OPA_50, 0);

    _nav_traffic_img = lv_img_create(_nav_traffic_card);
    lv_img_set_src(_nav_traffic_img, &nav_traffic_red);
    lv_obj_align(_nav_traffic_img, LV_ALIGN_LEFT_MID, 16, 0);

    _nav_traffic_title = cjk_label(_nav_traffic_card, "前方红灯 80m", kRed);
    lv_obj_align(_nav_traffic_title, LV_ALIGN_TOP_LEFT, 60, 16);
    _nav_traffic_countdown = label(_nav_traffic_card, "16s", &lv_font_montserrat_28, kRed);
    lv_obj_align(_nav_traffic_countdown, LV_ALIGN_BOTTOM_MID, 18, -12);

    resetNavigationState(_navigation_state);
    _nav_rendered_light_state = NavigationLightState::None;
    updateNavigationPage();
}

void LensReactUI::updateNavigationPage(void)
{
    if(_nav_direction_img == nullptr || _nav_distance_label == nullptr || _nav_speed_label == nullptr) {
        return;
    }

    char value[64];
    std::snprintf(value, sizeof(value), "%d", _navigation_state.distance);
    lv_label_set_text(_nav_distance_label, value);
    std::snprintf(value, sizeof(value), "%d", _navigation_state.speed);
    lv_label_set_text(_nav_speed_label, value);
    lv_img_set_src(_nav_direction_img, navigation_direction_image(_navigation_state.direction));

    const bool stopped = _navigation_state.speed == 0;
    const lv_color_t active_color = stopped ? kRed : kHudGreen;
    const lv_color_t dot_color = stopped ? kRed : kHudDot;
    lv_obj_set_style_text_color(_nav_speed_label, active_color, 0);
    if(_nav_baseline) {
        lv_obj_set_style_bg_color(_nav_baseline, active_color, 0);
    }
    if(_nav_start_dot) {
        lv_obj_set_style_bg_color(_nav_start_dot, dot_color, 0);
    }
    if(_nav_end_dot) {
        lv_obj_set_style_bg_color(_nav_end_dot, dot_color, 0);
    }

    if(_nav_traffic_card == nullptr) {
        return;
    }
    if(_navigation_state.light_state == NavigationLightState::None) {
        if(_nav_rendered_light_state != NavigationLightState::None) {
            lv_anim_t fade;
            lv_anim_init(&fade);
            lv_anim_set_var(&fade, _nav_traffic_card);
            lv_anim_set_values(&fade, LV_OPA_COVER, LV_OPA_TRANSP);
            lv_anim_set_time(&fade, 500);
            lv_anim_set_path_cb(&fade, lv_anim_path_ease_in);
            lv_anim_set_exec_cb(&fade, onAnimOpa);
            lv_anim_set_ready_cb(&fade, onAnimHideReady);
            lv_anim_start(&fade);

        } else {
            lv_obj_add_flag(_nav_traffic_card, LV_OBJ_FLAG_HIDDEN);
        }
        _nav_rendered_light_state = NavigationLightState::None;
        return;
    }

    const bool show_animation = _nav_rendered_light_state == NavigationLightState::None;
    lv_obj_clear_flag(_nav_traffic_card, LV_OBJ_FLAG_HIDDEN);
    if(show_animation) {
        lv_obj_set_style_opa(_nav_traffic_card, LV_OPA_TRANSP, 0);

        lv_anim_t fade;
        lv_anim_init(&fade);
        lv_anim_set_var(&fade, _nav_traffic_card);
        lv_anim_set_values(&fade, LV_OPA_TRANSP, LV_OPA_COVER);
        lv_anim_set_time(&fade, 500);
        lv_anim_set_path_cb(&fade, lv_anim_path_ease_out);
        lv_anim_set_exec_cb(&fade, onAnimOpa);
        lv_anim_start(&fade);
    }
    _nav_rendered_light_state = _navigation_state.light_state;

    const bool red = _navigation_state.light_state == NavigationLightState::Red;
    const lv_color_t light_color = red ? kRed : kGreen;
    lv_obj_set_style_border_color(_nav_traffic_card, light_color, 0);
    lv_img_set_src(_nav_traffic_img, red ? &nav_traffic_red : &nav_traffic_green);
    lv_obj_set_style_text_color(_nav_traffic_title, light_color, 0);
    lv_obj_set_style_text_color(_nav_traffic_countdown, light_color, 0);

    if(red) {
        if(_navigation_state.light_distance > 0) {
            std::snprintf(value, sizeof(value), "前方红灯 %dm", _navigation_state.light_distance);
        } else {
            std::snprintf(value, sizeof(value), "红灯，请停止");
        }
        lv_label_set_text(_nav_traffic_title, value);
        std::snprintf(value, sizeof(value), "%ds", _navigation_state.light_countdown);
        lv_label_set_text(_nav_traffic_countdown, value);
        lv_obj_clear_flag(_nav_traffic_countdown, LV_OBJ_FLAG_HIDDEN);
    } else {
        lv_label_set_text(_nav_traffic_title, "绿灯通行");
        lv_obj_add_flag(_nav_traffic_countdown, LV_OBJ_FLAG_HIDDEN);
    }
}

void LensReactUI::createNotificationPage(void)
{
    _notification_stack = lv_obj_create(_page_content);
    set_plain(_notification_stack);
    lv_obj_set_size(_notification_stack, 280, 300);
    lv_obj_align(_notification_stack, LV_ALIGN_TOP_LEFT, 16, 48);
    lv_obj_set_style_bg_opa(_notification_stack, LV_OPA_TRANSP, 0);
    lv_obj_clear_flag(_notification_stack, LV_OBJ_FLAG_CLICKABLE);

    lv_obj_t *empty_pill = box(_page_content, 248, 42, LV_COLOR_MAKE(0x11, 0x11, 0x14), LV_OPA_70, 21);
    lv_obj_align(empty_pill, LV_ALIGN_CENTER, 0, 6);
    lv_obj_set_style_border_width(empty_pill, 1, 0);
    lv_obj_set_style_border_color(empty_pill, kWhite, 0);
    lv_obj_set_style_border_opa(empty_pill, LV_OPA_10, 0);

    _notification_empty_label = cjk_label(empty_pill, "按 1、2、3 键模拟新通知", kTextFaint);
    lv_obj_center(_notification_empty_label);
    updateNotificationBubbleLayout();
}

void LensReactUI::clearNotificationBubbles(void)
{
    for(auto &bubble : _notification_bubbles) {
        if(bubble.obj) {
            lv_obj_del(bubble.obj);
            bubble.obj = nullptr;
        }
        bubble.timeline = {};
        bubble.fade_started = false;
        bubble.exit_started = false;
    }
}

void LensReactUI::triggerNotification(uint8_t type)
{
    struct NotificationSample {
        const char *source;
        const char *sender;
        const char *message;
        const char *icon;
        lv_color_t accent;
    };
    const NotificationSample samples[] = {
        {"微信", "张三", "晚饭定在科技园那家日料店了。", LV_SYMBOL_BELL, kGreen},
        {"短信", "中国移动", "验证码 482916，5 分钟内有效。", LV_SYMBOL_CALL, kBlue},
        {"钉钉", "项目会议", "15:30 产品评审会即将开始。", LV_SYMBOL_LIST, kOrange},
    };

    if(type >= (sizeof(samples) / sizeof(samples[0]))) {
        return;
    }
    addNotificationBubble(samples[type].source, samples[type].sender, samples[type].message, samples[type].icon,
                          samples[type].accent);
}

void LensReactUI::addNotificationBubble(const char *source, const char *sender, const char *message, const char *icon,
                                        lv_color_t accent)
{
    if(_notification_stack == nullptr) {
        return;
    }

    if(_notification_bubbles[kMaxNotificationBubbles - 1].obj) {
        lv_obj_del(_notification_bubbles[kMaxNotificationBubbles - 1].obj);
    }
    for(int8_t i = kMaxNotificationBubbles - 1; i > 0; --i) {
        _notification_bubbles[i] = _notification_bubbles[i - 1];
    }
    _notification_bubbles[0] = {};

    const lv_coord_t bubble_width = 260;
    const lv_coord_t bubble_height = 60;
    lv_obj_t *bubble = box(_notification_stack, bubble_width, bubble_height,
                           LV_COLOR_MAKE(0x0d, 0x0e, 0x11), LV_OPA_80, 15);
    lv_obj_set_style_border_width(bubble, 1, 0);
    lv_obj_set_style_border_color(bubble, kWhite, 0);
    lv_obj_set_style_border_opa(bubble, LV_OPA_20, 0);
    lv_obj_set_style_shadow_width(bubble, 14, 0);
    lv_obj_set_style_shadow_color(bubble, kBlack, 0);
    lv_obj_set_style_shadow_opa(bubble, LV_OPA_50, 0);

    lv_obj_t *badge = box(bubble, 40, 40, accent, LV_OPA_20, 10);
    lv_obj_set_pos(badge, 10, 10);
    lv_obj_set_style_border_width(badge, 1, 0);
    lv_obj_set_style_border_color(badge, accent, 0);
    lv_obj_set_style_border_opa(badge, LV_OPA_40, 0);
    lv_obj_t *badge_icon = label(badge, icon, &lv_font_montserrat_16, accent);
    lv_obj_center(badge_icon);

    lv_obj_t *sender_label = label(bubble, sender, &smartglass_font_14_cjk, kText);
    lv_obj_set_pos(sender_label, 62, 8);

    lv_obj_t *message_label = label(bubble, message, &smartglass_font_12_cjk, kTextDim);
    lv_obj_set_width(message_label, bubble_width - 74);
    lv_label_set_long_mode(message_label, LV_LABEL_LONG_DOT);
    lv_obj_set_pos(message_label, 62, 32);

    lv_obj_t *source_label = label(bubble, source, &smartglass_font_12_cjk, kTextFaint);
    lv_obj_align(source_label, LV_ALIGN_TOP_RIGHT, -10, 9);

    _notification_bubbles[0].obj = bubble;
    resetNotificationTimeline(_notification_bubbles[0].timeline);
    updateNotificationBubbleLayout();
    if(_notification_bubbles[kMaxNotificationBubbles - 1].obj) {
        startNotificationBubbleExit(kMaxNotificationBubbles - 1);
    }

    lv_obj_set_x(bubble, -bubble_width - 20);
    lv_obj_set_style_opa(bubble, LV_OPA_TRANSP, 0);

    lv_anim_t slide;
    lv_anim_init(&slide);
    lv_anim_set_var(&slide, bubble);
    lv_anim_set_values(&slide, lv_obj_get_x(bubble), 0);
    lv_anim_set_time(&slide, 200);
    lv_anim_set_path_cb(&slide, lv_anim_path_ease_out);
    lv_anim_set_exec_cb(&slide, onAnimX);
    lv_anim_start(&slide);

    lv_anim_t fade;
    lv_anim_init(&fade);
    lv_anim_set_var(&fade, bubble);
    lv_anim_set_values(&fade, LV_OPA_TRANSP, LV_OPA_COVER);
    lv_anim_set_time(&fade, 200);
    lv_anim_set_exec_cb(&fade, onAnimOpa);
    lv_anim_start(&fade);
}

void LensReactUI::startNotificationBubbleFade(uint8_t index)
{
    if(index >= kMaxNotificationBubbles) {
        return;
    }
    auto &item = _notification_bubbles[index];
    if(item.obj == nullptr || item.fade_started || item.exit_started) {
        return;
    }
    item.fade_started = true;

    lv_anim_t fade;
    lv_anim_init(&fade);
    lv_anim_set_var(&fade, item.obj);
    lv_anim_set_values(&fade, lv_obj_get_style_opa(item.obj, 0), LV_OPA_60);
    lv_anim_set_time(&fade, 1000);
    lv_anim_set_path_cb(&fade, lv_anim_path_linear);
    lv_anim_set_exec_cb(&fade, onAnimOpa);
    lv_anim_start(&fade);
}

void LensReactUI::startNotificationBubbleExit(uint8_t index)
{
    if(index >= kMaxNotificationBubbles) {
        return;
    }
    auto &item = _notification_bubbles[index];
    if(item.obj == nullptr || item.exit_started) {
        return;
    }
    item.exit_started = true;
    forceExitNotificationTimeline(item.timeline);
    lv_anim_del(item.obj, onAnimX);
    lv_anim_del(item.obj, onAnimOpa);

    lv_anim_t slide;
    lv_anim_init(&slide);
    lv_anim_set_var(&slide, item.obj);
    lv_anim_set_values(&slide, lv_obj_get_x(item.obj), -280);
    lv_anim_set_time(&slide, 240);
    lv_anim_set_path_cb(&slide, lv_anim_path_ease_in);
    lv_anim_set_exec_cb(&slide, onAnimX);
    lv_anim_start(&slide);

    lv_anim_t fade;
    lv_anim_init(&fade);
    lv_anim_set_var(&fade, item.obj);
    lv_anim_set_values(&fade, lv_obj_get_style_opa(item.obj, 0), LV_OPA_TRANSP);
    lv_anim_set_time(&fade, 240);
    lv_anim_set_path_cb(&fade, lv_anim_path_ease_in);
    lv_anim_set_exec_cb(&fade, onAnimOpa);
    lv_anim_start(&fade);
}

void LensReactUI::removeNotificationBubble(uint8_t index)
{
    if(index >= kMaxNotificationBubbles || _notification_bubbles[index].obj == nullptr) {
        return;
    }
    lv_obj_del(_notification_bubbles[index].obj);
    for(uint8_t i = index; i + 1 < kMaxNotificationBubbles; ++i) {
        _notification_bubbles[i] = _notification_bubbles[i + 1];
    }
    _notification_bubbles[kMaxNotificationBubbles - 1] = {};
    updateNotificationBubbleLayout();
}

void LensReactUI::updateNotificationBubbleLayout(void)
{
    constexpr lv_coord_t bubble_height = 60;
    constexpr lv_coord_t gap = 12;
    bool has_bubbles = false;

    for(uint8_t i = 0; i < kMaxNotificationBubbles; ++i) {
        lv_obj_t *bubble = _notification_bubbles[i].obj;
        if(bubble == nullptr) {
            continue;
        }
        has_bubbles = true;
        const lv_coord_t target_y = i * (bubble_height + gap);
        if(!_notification_bubbles[i].exit_started && lv_obj_get_y(bubble) != target_y) {
            lv_anim_t move;
            lv_anim_init(&move);
            lv_anim_set_var(&move, bubble);
            lv_anim_set_values(&move, lv_obj_get_y(bubble), target_y);
            lv_anim_set_time(&move, 150);
            lv_anim_set_path_cb(&move, lv_anim_path_ease_out);
            lv_anim_set_exec_cb(&move, onAnimY);
            lv_anim_start(&move);
        }
    }

    if(_notification_empty_label) {
        lv_obj_t *empty_pill = lv_obj_get_parent(_notification_empty_label);
        if(has_bubbles) {
            lv_obj_add_flag(empty_pill, LV_OBJ_FLAG_HIDDEN);
        } else {
            lv_obj_clear_flag(empty_pill, LV_OBJ_FLAG_HIDDEN);
        }
    }
}

void LensReactUI::createMusicPage(void)
{
    const bool compact = _view_height <= kCompactViewMaxHeight;
    const lv_coord_t cover_size = compact ? 96 : 148;
    const lv_coord_t cover_y = compact ? 54 : 96;
    const lv_coord_t title_y = compact ? 160 : 272;
    const lv_coord_t progress_y = compact ? 226 : 368;
    const lv_coord_t controls_y = compact ? 270 : 432;

    _music_cover = box(_page_content, cover_size, cover_size, kPurple, LV_OPA_COVER, compact ? 18 : 22);
    lv_obj_align(_music_cover, LV_ALIGN_TOP_MID, 0, cover_y);
    lv_obj_set_style_bg_grad_color(_music_cover, kCyan, 0);
    lv_obj_set_style_bg_grad_dir(_music_cover, LV_GRAD_DIR_VER, 0);
    lv_obj_set_style_shadow_width(_music_cover, compact ? 14 : 22, 0);
    lv_obj_set_style_shadow_color(_music_cover, LV_COLOR_MAKE(0x03, 0x10, 0x18), 0);
    lv_obj_set_style_shadow_opa(_music_cover, LV_OPA_50, 0);
    lv_obj_t *music = label(_music_cover, LV_SYMBOL_AUDIO, compact ? &lv_font_montserrat_34 : &lv_font_montserrat_48,
                            LV_COLOR_MAKE(0xd8, 0xe5, 0xff));
    lv_obj_center(music);

    _music_title = label(_page_content, "", compact ? &lv_font_montserrat_24 : &lv_font_montserrat_28, kText);
    lv_obj_set_width(_music_title, _view_width - 120);
    lv_label_set_long_mode(_music_title, LV_LABEL_LONG_DOT);
    lv_obj_set_style_text_align(_music_title, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(_music_title, LV_ALIGN_TOP_MID, 0, title_y);

    _music_artist = label(_page_content, "", compact ? &lv_font_montserrat_14 : &lv_font_montserrat_16, kTextDim);
    lv_obj_set_width(_music_artist, _view_width - 120);
    lv_label_set_long_mode(_music_artist, LV_LABEL_LONG_DOT);
    lv_obj_set_style_text_align(_music_artist, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(_music_artist, LV_ALIGN_TOP_MID, 0, title_y + (compact ? 30 : 38));

    const lv_coord_t progress_width = compact ? 230 : 256;
    lv_obj_t *progress_track = box(_page_content, progress_width, 5, LV_COLOR_MAKE(0x2c, 0x2c, 0x2f), LV_OPA_COVER, 3);
    lv_obj_align(progress_track, LV_ALIGN_TOP_MID, 0, progress_y);
    _music_progress_fill = box(progress_track, 1, 5, kText, LV_OPA_COVER, 3);
    lv_obj_align(_music_progress_fill, LV_ALIGN_LEFT_MID, 0, 0);

    _music_elapsed_label = label(_page_content, "", &lv_font_montserrat_12, kTextDim);
    lv_obj_set_width(_music_elapsed_label, 40);
    lv_obj_set_style_text_align(_music_elapsed_label, LV_TEXT_ALIGN_RIGHT, 0);
    lv_obj_align_to(_music_elapsed_label, progress_track, LV_ALIGN_OUT_LEFT_MID, -22, 0);
    _music_duration_label = label(_page_content, "", &lv_font_montserrat_12, kTextDim);
    lv_obj_set_width(_music_duration_label, 40);
    lv_obj_align_to(_music_duration_label, progress_track, LV_ALIGN_OUT_RIGHT_MID, 22, 0);

    const lv_coord_t side_offset = compact ? 74 : 88;
    const lv_coord_t side_size = compact ? 44 : 54;
    const lv_coord_t play_size = compact ? 58 : 66;
    lv_obj_t *prev_btn = box(_page_content, side_size, side_size, kBlack, LV_OPA_TRANSP, LV_RADIUS_CIRCLE);
    lv_obj_align(prev_btn, LV_ALIGN_TOP_MID, -side_offset, controls_y);
    lv_obj_add_flag(prev_btn, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_user_data(prev_btn, reinterpret_cast<void *>(static_cast<uintptr_t>(0)));
    lv_obj_add_event_cb(prev_btn, onMusicControlClicked, LV_EVENT_CLICKED, this);
    lv_obj_t *prev = label(prev_btn, LV_SYMBOL_PREV, compact ? &lv_font_montserrat_24 : &lv_font_montserrat_28,
                           kTextDim);
    lv_obj_center(prev);
    lv_obj_clear_flag(prev, LV_OBJ_FLAG_CLICKABLE);

    lv_obj_t *play_btn = box(_page_content, play_size, play_size, kText, LV_OPA_COVER, LV_RADIUS_CIRCLE);
    lv_obj_align(play_btn, LV_ALIGN_TOP_MID, 0, controls_y - ((play_size - side_size) / 2));
    lv_obj_add_flag(play_btn, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_user_data(play_btn, reinterpret_cast<void *>(static_cast<uintptr_t>(1)));
    lv_obj_add_event_cb(play_btn, onMusicControlClicked, LV_EVENT_CLICKED, this);
    _music_play_icon = label(play_btn, LV_SYMBOL_PAUSE, compact ? &lv_font_montserrat_28 : &lv_font_montserrat_34,
                             kBlack);
    lv_obj_center(_music_play_icon);
    lv_obj_clear_flag(_music_play_icon, LV_OBJ_FLAG_CLICKABLE);

    lv_obj_t *next_btn = box(_page_content, side_size, side_size, kBlack, LV_OPA_TRANSP, LV_RADIUS_CIRCLE);
    lv_obj_align(next_btn, LV_ALIGN_TOP_MID, side_offset, controls_y);
    lv_obj_add_flag(next_btn, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_user_data(next_btn, reinterpret_cast<void *>(static_cast<uintptr_t>(2)));
    lv_obj_add_event_cb(next_btn, onMusicControlClicked, LV_EVENT_CLICKED, this);
    lv_obj_t *next = label(next_btn, LV_SYMBOL_NEXT, compact ? &lv_font_montserrat_24 : &lv_font_montserrat_28,
                           kTextDim);
    lv_obj_center(next);
    lv_obj_clear_flag(next, LV_OBJ_FLAG_CLICKABLE);

    lv_obj_t *hint = cjk_label(_page_content, "‹ 按 ← → 键切歌，Enter 键播放/暂停 ›", kTextFaint);
    lv_obj_set_width(hint, _view_width - 80);
    lv_label_set_long_mode(hint, LV_LABEL_LONG_DOT);
    lv_obj_set_style_text_align(hint, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(hint, LV_ALIGN_BOTTOM_MID, 0, compact ? -14 : -30);

    updateMusicPage();
}

void LensReactUI::updateMusicPage(void)
{
    if(_music_title == nullptr || _music_artist == nullptr || _music_elapsed_label == nullptr ||
       _music_duration_label == nullptr || _music_progress_fill == nullptr || _music_play_icon == nullptr) {
        return;
    }
    const MusicTrack &track = kMusicTracks[_music_track_index % kMusicTrackCount];
    if(_music_elapsed_seconds > track.duration_seconds) {
        _music_elapsed_seconds = track.duration_seconds;
    }

    lv_label_set_text(_music_title, track.title);
    lv_label_set_text(_music_artist, track.artist);
    if(_music_cover) {
        lv_obj_set_style_bg_color(_music_cover, track.cover_start, 0);
        lv_obj_set_style_bg_grad_color(_music_cover, track.cover_end, 0);
    }
    lv_label_set_text(_music_play_icon, _music_playing ? LV_SYMBOL_PAUSE : LV_SYMBOL_PLAY);

    char elapsed[8];
    char duration[8];
    format_time(elapsed, sizeof(elapsed), _music_elapsed_seconds);
    format_time(duration, sizeof(duration), track.duration_seconds);
    lv_label_set_text(_music_elapsed_label, elapsed);
    lv_label_set_text(_music_duration_label, duration);

    lv_obj_t *progress_track = lv_obj_get_parent(_music_progress_fill);
    const lv_coord_t track_width = progress_track ? lv_obj_get_width(progress_track) : 0;
    const lv_coord_t fill_width = track.duration_seconds == 0 ?
                                      1 :
                                      LV_MAX((lv_coord_t)1,
                                             (lv_coord_t)((track_width * _music_elapsed_seconds) /
                                                          track.duration_seconds));
    lv_obj_set_width(_music_progress_fill, fill_width);
    lv_obj_align(_music_progress_fill, LV_ALIGN_LEFT_MID, 0, 0);
}

void LensReactUI::toggleMusicPlayback(void)
{
    _music_playing = !_music_playing;
    updateMusicPage();
}

void LensReactUI::changeMusicTrack(int8_t delta)
{
    const int next = (static_cast<int>(_music_track_index) + static_cast<int>(delta) + kMusicTrackCount) %
                     kMusicTrackCount;
    _music_track_index = static_cast<uint8_t>(next);
    _music_elapsed_seconds = _music_track_index == 0 ? 72 : 0;
    _music_playing = true;
    updateMusicPage();
}

void LensReactUI::createPrompterPage(void)
{
    const bool compact = _view_height <= kCompactViewMaxHeight || _view_width <= 700;

    createSessionTimerPill(LV_ALIGN_TOP_LEFT, compact ? 16 : 24, compact ? 38 : 48);

    _prompt_cursor = box(_page_content, 3, compact ? 16 : 20, kGreen, LV_OPA_COVER, 2);
    lv_obj_align(_prompt_cursor, LV_ALIGN_TOP_LEFT, compact ? 18 : 24, compact ? 76 : 90);

    _prompt_text_label = label(_page_content, "", compact ? &smartglass_font_14_cjk : &smartglass_font_16_cjk,
                               kText);
    lv_obj_set_width(_prompt_text_label, compact ? (_view_width - 56) : LV_MIN((lv_coord_t)944,
                                                                                (lv_coord_t)(_view_width - 80)));
    lv_label_set_long_mode(_prompt_text_label, LV_LABEL_LONG_WRAP);
    lv_label_set_recolor(_prompt_text_label, true);
    lv_obj_set_style_text_align(_prompt_text_label, LV_TEXT_ALIGN_LEFT, 0);
    lv_obj_set_style_text_line_space(_prompt_text_label, compact ? 8 : 12, 0);
    lv_obj_align(_prompt_text_label, LV_ALIGN_CENTER, 0, compact ? 20 : 28);

    _prompt_focus = 0;
    updatePrompterPage();
}

void LensReactUI::updatePrompterPage(void)
{
    if(_prompt_text_label == nullptr) {
        return;
    }

    char text[1536] = {};
    size_t used = 0;
    for(uint8_t i = 0; i < kPrompterSegmentCount; ++i) {
        const char *segment = kPrompterSegments[i];
        const int written = i == _prompt_focus
                                ? std::snprintf(text + used, sizeof(text) - used, "#34d399 %s#", segment)
                                : std::snprintf(text + used, sizeof(text) - used, "%s", segment);
        if(written < 0 || static_cast<size_t>(written) >= sizeof(text) - used) {
            break;
        }
        used += static_cast<size_t>(written);
    }
    lv_label_set_text(_prompt_text_label, text);
    if(_prompt_timer) {
        lv_timer_set_period(_prompt_timer, prompter_segment_duration(kPrompterSegments[_prompt_focus]));
    }
}

void LensReactUI::createSessionTimerPill(lv_align_t align, lv_coord_t x, lv_coord_t y)
{
    _session_timer_pill = box(_page_content, 94, 26, kBlack, LV_OPA_60, 13);
    lv_obj_align(_session_timer_pill, align, x, y);
    lv_obj_set_style_border_width(_session_timer_pill, 1, 0);
    lv_obj_set_style_border_color(_session_timer_pill, kWhite, 0);
    lv_obj_set_style_border_opa(_session_timer_pill, LV_OPA_10, 0);
    lv_obj_set_style_shadow_width(_session_timer_pill, 10, 0);
    lv_obj_set_style_shadow_color(_session_timer_pill, kBlack, 0);
    lv_obj_set_style_shadow_opa(_session_timer_pill, LV_OPA_40, 0);

    lv_obj_t *divider = box(_session_timer_pill, 1, 14, kCyan, LV_OPA_70, 1);
    lv_obj_align(divider, LV_ALIGN_LEFT_MID, 0, 0);

    lv_obj_t *icon = lv_img_create(_session_timer_pill);
    lv_img_set_src(icon, &session_clock_icon);
    style_no_frame(icon);
    lv_obj_set_style_img_recolor(icon, kCyan, 0);
    lv_obj_set_style_img_recolor_opa(icon, LV_OPA_COVER, 0);
    lv_obj_align(icon, LV_ALIGN_LEFT_MID, 16, 0);
    lv_obj_clear_flag(icon, LV_OBJ_FLAG_CLICKABLE);

    _session_elapsed_label = label(_session_timer_pill, "0:00", &lv_font_montserrat_12, kCyan);
    lv_obj_set_style_text_font(_session_elapsed_label, &lv_font_montserrat_12, 0);
    lv_obj_align(_session_elapsed_label, LV_ALIGN_LEFT_MID, 35, 0);

    resetSessionTimer();
}

void LensReactUI::resetSessionTimer(void)
{
    _session_start_tick = lv_tick_get();
    if(_session_start_tick == 0) {
        _session_start_tick = 1;
    }
    updateSessionTimer();
}

void LensReactUI::updateSessionTimer(void)
{
    if(_session_elapsed_label == nullptr || _session_start_tick == 0) {
        return;
    }

    const uint32_t elapsed_seconds = lv_tick_elaps(_session_start_tick) / 1000;
    char elapsed[12];
    std::snprintf(elapsed, sizeof(elapsed), "%u:%02u",
                  static_cast<unsigned>(elapsed_seconds / 60),
                  static_cast<unsigned>(elapsed_seconds % 60));
    lv_label_set_text(_session_elapsed_label, elapsed);
}

void LensReactUI::createTranslatePage(void)
{
    _translate_stack = lv_obj_create(_page_content);
    set_plain(_translate_stack);
    lv_obj_set_size(_translate_stack, _view_width, _view_height - 38);
    lv_obj_align(_translate_stack, LV_ALIGN_TOP_MID, 0, 38);
    lv_obj_set_style_bg_opa(_translate_stack, LV_OPA_TRANSP, 0);

    lv_obj_t *empty_pill = box(_page_content, 270, 42, LV_COLOR_MAKE(0x08, 0x30, 0x38), LV_OPA_40, 21);
    lv_obj_align(empty_pill, LV_ALIGN_CENTER, 0, 18);
    lv_obj_set_style_border_width(empty_pill, 1, 0);
    lv_obj_set_style_border_color(empty_pill, kCyan, 0);
    lv_obj_set_style_border_opa(empty_pill, LV_OPA_30, 0);
    lv_obj_t *empty_icon = label(empty_pill, LV_SYMBOL_GPS, &lv_font_montserrat_14, kCyan);
    lv_obj_align(empty_icon, LV_ALIGN_LEFT_MID, 18, 0);
    _translate_empty = label(empty_pill, "按 1、2、3 键模拟语音输入", &smartglass_font_14_cjk, kCyan);
    lv_obj_align(_translate_empty, LV_ALIGN_LEFT_MID, 42, 0);

}

void LensReactUI::createRealtimeTranslatePage(void)
{
    const bool compact = _view_height <= kCompactViewMaxHeight || _view_width <= 700;

#if SMARTGLASS_ENABLE_SIMULATION_BACKGROUNDS
    cover_image(_page_content, &realtime_translate_background, _view_width, _view_height);
    lv_obj_t *shade = box(_page_content, _view_width, _view_height, kBlack, LV_OPA_50, 0);
    lv_obj_center(shade);
    lv_obj_clear_flag(shade, LV_OBJ_FLAG_CLICKABLE);
#else
    lv_obj_t *shade = box(_page_content, _view_width, _view_height, LV_COLOR_MAKE(0x05, 0x06, 0x07), LV_OPA_COVER, 0);
    lv_obj_center(shade);
    lv_obj_clear_flag(shade, LV_OBJ_FLAG_CLICKABLE);
#endif

    _realtime_source_label = label(_page_content, "", &lv_font_montserrat_14, kTextDim);
    lv_obj_set_width(_realtime_source_label, _view_width - (compact ? 48 : 96));
    lv_label_set_long_mode(_realtime_source_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_style_text_align(_realtime_source_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(_realtime_source_label, LV_ALIGN_BOTTOM_MID, 0, compact ? -72 : -118);

    _realtime_translation_label = label(_page_content, "", &smartglass_font_20_cjk, kGreen);
    lv_obj_set_width(_realtime_translation_label, _view_width - (compact ? 48 : 96));
    lv_label_set_long_mode(_realtime_translation_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_style_text_align(_realtime_translation_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(_realtime_translation_label, LV_ALIGN_BOTTOM_MID, 0, compact ? -42 : -78);

    _realtime_sentence_index = 0;
    _realtime_stage = 0;
    _realtime_delay = 24;
    _realtime_progress = 0;
    updateRealtimeTranslatePage();
}

void LensReactUI::updateRealtimeTranslatePage(void)
{
    if(_realtime_source_label == nullptr || _realtime_translation_label == nullptr) {
        return;
    }

    struct Sample {
        const char *source;
        const char *translation;
    };
    const Sample samples[] = {
        {"We need to memorize these basic structures and their directing effects.",
         "我们需要记住这些基本结构及其导向效应。"},
        {"So that we can better predict and design organic reactions.",
         "这样我们才能更好地预测和设计有机反应。"},
        {"Are there any questions about this part of the lecture?",
         "关于这部分讲座有什么问题吗？"},
        {"The next example shows how the reaction changes under different conditions.",
         "下一个例子展示了反应在不同条件下的变化。"},
    };
    constexpr uint8_t sample_count = sizeof(samples) / sizeof(samples[0]);
    const Sample &sample = samples[_realtime_sentence_index % sample_count];

    if(_realtime_stage == 0) {
        lv_label_set_text(_realtime_source_label, "Listening to the environment...");
        lv_label_set_text(_realtime_translation_label, "正在监听环境语音...");
        if(_realtime_delay > 0) {
            _realtime_delay--;
            return;
        }
        _realtime_stage = 1;
        _realtime_progress = 0;
    }

    if(_realtime_stage == 1) {
        const uint16_t total = utf8_char_count(sample.source);
        _realtime_progress = LV_MIN((uint16_t)(_realtime_progress + 2), total);
        set_label_utf8_prefix(_realtime_source_label, sample.source, _realtime_progress);
        lv_label_set_text(_realtime_translation_label, "");
        if(_realtime_progress >= total) {
            _realtime_stage = 2;
            _realtime_progress = 0;
        }
        return;
    }

    if(_realtime_stage == 2) {
        const uint16_t total = utf8_char_count(sample.translation);
        lv_label_set_text(_realtime_source_label, sample.source);
        _realtime_progress = LV_MIN((uint16_t)(_realtime_progress + 1), total);
        set_label_utf8_prefix(_realtime_translation_label, sample.translation, _realtime_progress);
        if(_realtime_progress >= total) {
            _realtime_stage = 3;
            _realtime_delay = 30;
        }
        return;
    }

    lv_label_set_text(_realtime_source_label, sample.source);
    lv_label_set_text(_realtime_translation_label, sample.translation);
    if(_realtime_delay > 0) {
        _realtime_delay--;
        return;
    }
    _realtime_sentence_index = (_realtime_sentence_index + 1) % sample_count;
    _realtime_stage = 1;
    _realtime_progress = 0;
}

void LensReactUI::clearTranslateItems(void)
{
    for(auto &item : _translate_items) {
        if(item.obj) {
            lv_obj_del(item.obj);
        }
        item = {};
    }
}

void LensReactUI::triggerTranslation(uint8_t type)
{
    struct Sample {
        const char *zh;
        const char *en;
    };
    const Sample samples[] = {
        {"请问去最近的地铁站怎么走？", "Excuse me, how do I get to the nearest subway station?"},
        {"这附近有推荐的咖啡店吗？", "Are there any recommended coffee shops nearby?"},
        {"麻烦帮我结账，可以用信用卡吗？", "Check, please. Can I pay by credit card?"},
    };
    if(type < 3) {
        addTranslateItem(samples[type].zh, samples[type].en);
    }
}

void LensReactUI::addTranslateItem(const char *zh, const char *en)
{
    if(_translate_stack == nullptr) {
        return;
    }
    if(_translate_items[kMaxTranslateItems - 1].obj) {
        lv_obj_t *oldest = _translate_items[kMaxTranslateItems - 1].obj;
        lv_anim_del(oldest, nullptr);
        lv_obj_add_flag(oldest, LV_OBJ_FLAG_HIDDEN);
        lv_obj_del_async(oldest);
    }
    if(_translate_items[0].obj) {
        resetNotificationTimeline(_translate_items[0].timeline);
    }
    for(int8_t i = kMaxTranslateItems - 1; i > 0; --i) {
        _translate_items[i] = _translate_items[i - 1];
    }
    _translate_items[0] = {};

    lv_obj_t *item = box(_translate_stack, 520, 188, LV_COLOR_MAKE(0x10, 0x13, 0x17), LV_OPA_80, 20);
    lv_obj_set_style_border_width(item, 1, 0);
    lv_obj_set_style_border_color(item, kCyan, 0);
    lv_obj_set_style_border_opa(item, LV_OPA_30, 0);
    lv_obj_set_style_shadow_width(item, 24, 0);
    lv_obj_set_style_shadow_color(item, kBlack, 0);
    lv_obj_set_style_shadow_opa(item, LV_OPA_50, 0);

    lv_obj_t *source_icon = label(item, LV_SYMBOL_AUDIO, &lv_font_montserrat_12, kTextDim);
    lv_obj_set_pos(source_icon, 20, 14);
    lv_obj_t *source_tag = label(item, "原文（中文）", &smartglass_font_12_cjk, kTextDim);
    lv_obj_set_pos(source_tag, 40, 14);
    lv_obj_t *source_text = label(item, zh, &smartglass_font_20_cjk, kText);
    lv_obj_set_pos(source_text, 20, 43);

    lv_obj_t *divider = box(item, 480, 1, kWhite, LV_OPA_20, 0);
    lv_obj_set_pos(divider, 20, 86);

    lv_obj_t *output_tag = label(item, LV_SYMBOL_OK "  English Translation", &lv_font_montserrat_12, kCyan);
    lv_obj_set_pos(output_tag, 20, 103);
    lv_obj_t *output_text = label(item, en, &lv_font_montserrat_16, LV_COLOR_MAKE(0xec, 0xfe, 0xff));
    lv_obj_set_width(output_text, 480);
    lv_label_set_long_mode(output_text, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(output_text, 20, 132);

    _translate_items[0].obj = item;
    resetNotificationTimeline(_translate_items[0].timeline);
    updateTranslateItemLayout();
    lv_obj_set_style_opa(item, LV_OPA_TRANSP, 0);
    lv_anim_t fade;
    lv_anim_init(&fade);
    lv_anim_set_var(&fade, item);
    lv_anim_set_values(&fade, LV_OPA_TRANSP, LV_OPA_COVER);
    lv_anim_set_time(&fade, 220);
    lv_anim_set_exec_cb(&fade, onAnimOpa);
    lv_anim_start(&fade);
}

void LensReactUI::removeTranslateItem(uint8_t index)
{
    if(index >= kMaxTranslateItems || _translate_items[index].obj == nullptr) {
        return;
    }
    lv_obj_t *expired = _translate_items[index].obj;
    lv_anim_del(expired, nullptr);
    lv_obj_add_flag(expired, LV_OBJ_FLAG_HIDDEN);
    for(uint8_t i = index; i + 1 < kMaxTranslateItems; ++i) {
        _translate_items[i] = _translate_items[i + 1];
    }
    _translate_items[kMaxTranslateItems - 1] = {};
    updateTranslateItemLayout();
    lv_obj_del_async(expired);
}

void LensReactUI::updateTranslateItemLayout(void)
{
    constexpr lv_coord_t positions[] = {144, -56, -244};
    bool has_items = false;
    for(uint8_t i = 0; i < kMaxTranslateItems; ++i) {
        lv_obj_t *item = _translate_items[i].obj;
        if(item == nullptr) {
            continue;
        }
        has_items = true;
        lv_obj_set_x(item, (_view_width - 520) / 2);
        lv_anim_t move;
        lv_anim_init(&move);
        lv_anim_set_var(&move, item);
        lv_anim_set_values(&move, lv_obj_get_y(item), positions[i]);
        lv_anim_set_time(&move, 220);
        lv_anim_set_path_cb(&move, lv_anim_path_ease_out);
        lv_anim_set_exec_cb(&move, onAnimY);
        lv_anim_start(&move);
        lv_obj_set_style_opa(item, i == 0 ? LV_OPA_COVER : (i == 1 ? LV_OPA_50 : LV_OPA_20), 0);
    }
    if(_translate_empty) {
        lv_obj_t *pill = lv_obj_get_parent(_translate_empty);
        if(has_items) {
            lv_obj_add_flag(pill, LV_OBJ_FLAG_HIDDEN);
        } else {
            lv_obj_clear_flag(pill, LV_OBJ_FLAG_HIDDEN);
        }
    }
}

void LensReactUI::createBilingualChatPage(void)
{
    const bool compact = _view_height <= kCompactViewMaxHeight || _view_width <= 700;

    _bilingual_stack = lv_obj_create(_page_content);
    set_plain(_bilingual_stack);
    lv_obj_set_size(_bilingual_stack, _view_width, _view_height);
    lv_obj_align(_bilingual_stack, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_set_style_bg_opa(_bilingual_stack, LV_OPA_TRANSP, 0);
    lv_obj_clear_flag(_bilingual_stack, LV_OBJ_FLAG_CLICKABLE);

    _bilingual_empty = box(_page_content, compact ? 160 : 180, compact ? 118 : 138, kBlack, LV_OPA_TRANSP, 0);
    lv_obj_align(_bilingual_empty, LV_ALIGN_CENTER, 0, compact ? 0 : 10);

    lv_obj_t *empty_card = box(_bilingual_empty, compact ? 82 : 96, compact ? 82 : 96,
                               LV_COLOR_MAKE(0x15, 0x1a, 0x1d), LV_OPA_70, compact ? 22 : 28);
    lv_obj_align(empty_card, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_set_style_border_width(empty_card, 1, 0);
    lv_obj_set_style_border_color(empty_card, kWhite, 0);
    lv_obj_set_style_border_opa(empty_card, LV_OPA_10, 0);
    lv_obj_set_style_shadow_width(empty_card, 26, 0);
    lv_obj_set_style_shadow_color(empty_card, kBlack, 0);
    lv_obj_set_style_shadow_opa(empty_card, LV_OPA_50, 0);

    lv_obj_t *empty_icon = label(empty_card, LV_SYMBOL_CALL, compact ? &lv_font_montserrat_28 : &lv_font_montserrat_34,
                                 LV_COLOR_MAKE(0x7d, 0xf9, 0xe2));
    lv_obj_center(empty_icon);
    lv_obj_t *empty_title = cjk_label(_bilingual_empty, "双语畅聊", kTextDim);
    lv_obj_align_to(empty_title, empty_card, LV_ALIGN_OUT_BOTTOM_MID, 0, compact ? 8 : 12);

    lv_obj_t *hint = box(_page_content, compact ? 304 : 360, compact ? 26 : 30,
                         LV_COLOR_MAKE(0x16, 0x18, 0x1c), LV_OPA_70, compact ? 13 : 15);
    lv_obj_align(hint, LV_ALIGN_TOP_MID, 0, compact ? 54 : 68);
    lv_obj_set_style_border_width(hint, 1, 0);
    lv_obj_set_style_border_color(hint, kWhite, 0);
    lv_obj_set_style_border_opa(hint, LV_OPA_10, 0);
    lv_obj_t *hint_text = label(hint, "按 1 模拟【我】说话，按 2 模拟【对方】说话",
                                &smartglass_font_12_cjk, kTextDim);
    lv_obj_center(hint_text);

}

void LensReactUI::clearBilingualChatItems(void)
{
    for(auto &item : _bilingual_items) {
        if(item.obj) {
            lv_obj_del(item.obj);
        }
        item = {};
    }
}

void LensReactUI::triggerBilingualChat(uint8_t type)
{
    struct Sample {
        const char *role;
        const char *primary;
        const char *secondary;
        bool mine;
    };
    const Sample mine_samples[] = {
        {"我说（中文）", "你好，很高兴认识你。", "Hello, nice to meet you.", true},
        {"我说（中文）", "请问去最近的地铁站怎么走？",
         "Excuse me, how do I get to the nearest subway station?", true},
        {"我说（中文）", "我们可以在门口集合。", "We can meet at the entrance.", true},
    };
    const Sample other_samples[] = {
        {"对方（English）", "Go straight and turn left at the corner.", "直走并在拐角处左转。", false},
        {"对方（English）", "The meeting starts in ten minutes.", "会议将在十分钟后开始。", false},
        {"对方（English）", "I can help you order coffee.", "我可以帮你点咖啡。", false},
    };
    static uint8_t mine_index = 0;
    static uint8_t other_index = 0;

    if(type == 0) {
        const Sample &sample = mine_samples[mine_index % (sizeof(mine_samples) / sizeof(mine_samples[0]))];
        mine_index++;
        addBilingualChatItem(sample.role, sample.primary, sample.secondary, sample.mine);
        return;
    }
    if(type == 1) {
        const Sample &sample = other_samples[other_index % (sizeof(other_samples) / sizeof(other_samples[0]))];
        other_index++;
        addBilingualChatItem(sample.role, sample.primary, sample.secondary, sample.mine);
    }
}

void LensReactUI::addBilingualChatItem(const char *role, const char *primary, const char *secondary, bool mine)
{
    if(_bilingual_stack == nullptr) {
        return;
    }
    finishBilingualChatTyping();
    if(_bilingual_items[kMaxBilingualChatItems - 1].obj) {
        lv_obj_t *oldest = _bilingual_items[kMaxBilingualChatItems - 1].obj;
        lv_anim_del(oldest, nullptr);
        lv_obj_add_flag(oldest, LV_OBJ_FLAG_HIDDEN);
        lv_obj_del_async(oldest);
    }
    for(int8_t i = kMaxBilingualChatItems - 1; i > 0; --i) {
        _bilingual_items[i] = _bilingual_items[i - 1];
    }
    _bilingual_items[0] = {};

    const bool compact = _view_height <= kCompactViewMaxHeight || _view_width <= 700;
    const lv_coord_t bubble_width = compact ? LV_MIN((lv_coord_t)(_view_width - 44), (lv_coord_t)306) :
                                             LV_MIN((lv_coord_t)((_view_width * 43) / 100), (lv_coord_t)430);
    const lv_coord_t bubble_height = compact ? 98 : 116;
    lv_color_t bubble_color = LV_COLOR_MAKE(0x03, 0x3f, 0x3e);
    lv_color_t accent = LV_COLOR_MAKE(0x2d, 0xf4, 0xd5);
    lv_color_t role_color = LV_COLOR_MAKE(0x8b, 0xf7, 0xe6);
    lv_color_t secondary_color = LV_COLOR_MAKE(0xc0, 0xff, 0xf4);
    if(mine) {
        bubble_color = LV_COLOR_MAKE(0x0e, 0x22, 0x43);
        accent = LV_COLOR_MAKE(0x63, 0x8f, 0xff);
        role_color = LV_COLOR_MAKE(0xa9, 0xc3, 0xff);
        secondary_color = LV_COLOR_MAKE(0xc9, 0xd8, 0xff);
    }

    lv_obj_t *bubble = box(_bilingual_stack, bubble_width, bubble_height, bubble_color, LV_OPA_80,
                           compact ? 14 : 18);
    lv_obj_set_style_border_width(bubble, 1, 0);
    lv_obj_set_style_border_color(bubble, accent, 0);
    lv_obj_set_style_border_opa(bubble, LV_OPA_30, 0);
    lv_obj_set_style_shadow_width(bubble, 20, 0);
    lv_obj_set_style_shadow_color(bubble, kBlack, 0);
    lv_obj_set_style_shadow_opa(bubble, LV_OPA_40, 0);

    lv_obj_t *role_icon = label(bubble, LV_SYMBOL_AUDIO, &lv_font_montserrat_12, accent);
    lv_obj_set_pos(role_icon, 14, 12);
    lv_obj_t *role_label = label(bubble, role, &smartglass_font_12_cjk, role_color);
    lv_obj_set_pos(role_label, 32, 11);

    lv_obj_t *primary_label = label(bubble, "", mine ? &smartglass_font_16_cjk : &lv_font_montserrat_16, kText);
    lv_obj_set_width(primary_label, bubble_width - 28);
    lv_label_set_long_mode(primary_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(primary_label, 14, compact ? 34 : 38);

    lv_obj_t *divider = box(bubble, bubble_width - 28, 1, kWhite, LV_OPA_10, 0);
    lv_obj_set_pos(divider, 14, compact ? 68 : 80);

    lv_obj_t *secondary_label = label(bubble, "", mine ? &lv_font_montserrat_12 : &smartglass_font_14_cjk,
                                      secondary_color);
    lv_obj_set_width(secondary_label, bubble_width - 28);
    lv_label_set_long_mode(secondary_label, LV_LABEL_LONG_DOT);
    lv_obj_set_pos(secondary_label, 14, compact ? 76 : 90);

    _bilingual_items[0].obj = bubble;
    _bilingual_items[0].primary_label = primary_label;
    _bilingual_items[0].secondary_label = secondary_label;
    _bilingual_items[0].primary_text = primary;
    _bilingual_items[0].secondary_text = secondary;
    _bilingual_items[0].primary_total = utf8_char_count(primary);
    _bilingual_items[0].secondary_total = utf8_char_count(secondary);
    _bilingual_items[0].typing_stage = 0;
    _bilingual_items[0].typing_delay = 4;
    _bilingual_items[0].mine = mine;

    const lv_coord_t target_x = mine ? (_view_width - bubble_width - (compact ? 18 : 34)) : (compact ? 18 : 34);
    lv_obj_set_pos(bubble, target_x, _view_height + 16);
    lv_obj_set_style_opa(bubble, LV_OPA_TRANSP, 0);
    trimBilingualChatOverflow();
    updateBilingualChatLayout();

    lv_anim_t fade;
    lv_anim_init(&fade);
    lv_anim_set_var(&fade, bubble);
    lv_anim_set_values(&fade, LV_OPA_TRANSP, LV_OPA_COVER);
    lv_anim_set_time(&fade, 220);
    lv_anim_set_exec_cb(&fade, onAnimOpa);
    lv_anim_start(&fade);
}

void LensReactUI::finishBilingualChatTyping(void)
{
    for(auto &item : _bilingual_items) {
        if(item.obj == nullptr) {
            continue;
        }
        if(item.primary_label && item.primary_text) {
            lv_label_set_text(item.primary_label, item.primary_text);
        }
        if(item.secondary_label && item.secondary_text) {
            lv_label_set_text(item.secondary_label, item.secondary_text);
        }
        item.primary_progress = item.primary_total;
        item.secondary_progress = item.secondary_total;
        item.typing_stage = 3;
        item.typing_delay = 0;
    }
}

void LensReactUI::updateBilingualChatTyping(void)
{
    BilingualChatItem &item = _bilingual_items[0];
    if(item.obj == nullptr || item.typing_stage >= 3) {
        return;
    }
    if(item.typing_delay > 0) {
        item.typing_delay--;
        return;
    }

    if(item.typing_stage == 0) {
        if(item.primary_progress < item.primary_total) {
            item.primary_progress++;
            set_label_utf8_prefix(item.primary_label, item.primary_text, item.primary_progress);
            return;
        }
        item.typing_stage = 1;
        item.typing_delay = 3;
        return;
    }

    if(item.typing_stage == 1) {
        item.typing_stage = 2;
        item.typing_delay = 1;
        return;
    }

    if(item.typing_stage == 2) {
        if(item.secondary_progress < item.secondary_total) {
            item.secondary_progress++;
            set_label_utf8_prefix(item.secondary_label, item.secondary_text, item.secondary_progress);
            return;
        }
        item.typing_stage = 3;
    }
}

void LensReactUI::trimBilingualChatOverflow(void)
{
    const bool compact = _view_height <= kCompactViewMaxHeight || _view_width <= 700;
    const lv_coord_t bubble_height = compact ? 98 : 116;
    const lv_coord_t gap = compact ? 10 : 12;
    const lv_coord_t newest_y = _view_height - bubble_height - (compact ? 26 : 42);
    const lv_coord_t min_y = kStatusBarHeight + 2;

    for(int8_t i = kMaxBilingualChatItems - 1; i >= 0; --i) {
        if(_bilingual_items[i].obj == nullptr) {
            continue;
        }
        const lv_coord_t target_y = newest_y - i * (bubble_height + gap);
        if(target_y >= min_y) {
            break;
        }
        lv_obj_t *expired = _bilingual_items[i].obj;
        lv_anim_del(expired, nullptr);
        lv_obj_add_flag(expired, LV_OBJ_FLAG_HIDDEN);
        lv_obj_del_async(expired);
        _bilingual_items[i] = {};
    }
}

void LensReactUI::removeBilingualChatItem(uint8_t index)
{
    if(index >= kMaxBilingualChatItems || _bilingual_items[index].obj == nullptr) {
        return;
    }
    lv_obj_t *expired = _bilingual_items[index].obj;
    lv_anim_del(expired, nullptr);
    lv_obj_add_flag(expired, LV_OBJ_FLAG_HIDDEN);
    for(uint8_t i = index; i + 1 < kMaxBilingualChatItems; ++i) {
        _bilingual_items[i] = _bilingual_items[i + 1];
    }
    _bilingual_items[kMaxBilingualChatItems - 1] = {};
    updateBilingualChatLayout();
    lv_obj_del_async(expired);
}

void LensReactUI::updateBilingualChatLayout(void)
{
    const bool compact = _view_height <= kCompactViewMaxHeight || _view_width <= 700;
    const lv_coord_t bubble_width = compact ? LV_MIN((lv_coord_t)(_view_width - 44), (lv_coord_t)306) :
                                             LV_MIN((lv_coord_t)((_view_width * 43) / 100), (lv_coord_t)430);
    const lv_coord_t bubble_height = compact ? 98 : 116;
    const lv_coord_t gap = compact ? 10 : 12;
    const lv_coord_t newest_y = _view_height - bubble_height - (compact ? 26 : 42);
    bool has_items = false;

    for(uint8_t i = 0; i < kMaxBilingualChatItems; ++i) {
        lv_obj_t *item = _bilingual_items[i].obj;
        if(item == nullptr) {
            continue;
        }
        has_items = true;
        const lv_coord_t target_x = _bilingual_items[i].mine ? (_view_width - bubble_width - (compact ? 18 : 34)) :
                                                               (compact ? 18 : 34);
        const lv_coord_t target_y = newest_y - i * (bubble_height + gap);

        lv_anim_t move_x;
        lv_anim_init(&move_x);
        lv_anim_set_var(&move_x, item);
        lv_anim_set_values(&move_x, lv_obj_get_x(item), target_x);
        lv_anim_set_time(&move_x, i == 0 ? 240 : 180);
        lv_anim_set_path_cb(&move_x, lv_anim_path_ease_out);
        lv_anim_set_exec_cb(&move_x, onAnimX);
        lv_anim_start(&move_x);

        lv_anim_t move_y;
        lv_anim_init(&move_y);
        lv_anim_set_var(&move_y, item);
        lv_anim_set_values(&move_y, lv_obj_get_y(item), target_y);
        lv_anim_set_time(&move_y, 180);
        lv_anim_set_path_cb(&move_y, lv_anim_path_ease_out);
        lv_anim_set_exec_cb(&move_y, onAnimY);
        lv_anim_start(&move_y);

        lv_obj_set_style_opa(item, LV_OPA_COVER, 0);
    }

    if(_bilingual_empty) {
        if(has_items) {
            lv_obj_add_flag(_bilingual_empty, LV_OBJ_FLAG_HIDDEN);
        } else {
            lv_obj_clear_flag(_bilingual_empty, LV_OBJ_FLAG_HIDDEN);
        }
    }
}

void LensReactUI::createMeetingPage(void)
{
    const bool compact = _view_height <= kCompactViewMaxHeight || _view_width <= 700;

#if SMARTGLASS_ENABLE_SIMULATION_BACKGROUNDS
    cover_image(_page_content, &meeting_background, _view_width, _view_height);
    lv_obj_t *meeting_shade = box(_page_content, _view_width, _view_height, kBlack, LV_OPA_30, 0);
    lv_obj_center(meeting_shade);
    lv_obj_clear_flag(meeting_shade, LV_OBJ_FLAG_CLICKABLE);
#else
    lv_obj_t *meeting_shade = box(_page_content, _view_width, _view_height, LV_COLOR_MAKE(0x04, 0x05, 0x06),
                                  LV_OPA_COVER, 0);
    lv_obj_center(meeting_shade);
    lv_obj_clear_flag(meeting_shade, LV_OBJ_FLAG_CLICKABLE);
#endif

    createSessionTimerPill(LV_ALIGN_TOP_LEFT, compact ? 14 : 24, compact ? 42 : 52);

    lv_obj_t *ai_pill = box(_page_content, compact ? 228 : 292, compact ? 30 : 32,
                            LV_COLOR_MAKE(0x08, 0x08, 0x0c), LV_OPA_TRANSP, compact ? 15 : 16);
    lv_obj_align(ai_pill, LV_ALIGN_TOP_MID, 0, compact ? 68 : 74);
    lv_obj_set_style_border_width(ai_pill, 1, 0);
    lv_obj_set_style_border_color(ai_pill, kWhite, 0);
    lv_obj_set_style_border_opa(ai_pill, LV_OPA_10, 0);

    lv_obj_t *ai_content = lv_obj_create(ai_pill);
    set_plain(ai_content);
    lv_obj_set_size(ai_content, lv_pct(100), 20);
    lv_obj_center(ai_content);
    lv_obj_set_style_bg_opa(ai_content, LV_OPA_TRANSP, 0);
    lv_obj_set_style_pad_column(ai_content, compact ? 6 : 8, 0);
    lv_obj_set_flex_flow(ai_content, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(ai_content, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_clear_flag(ai_content, LV_OBJ_FLAG_CLICKABLE);

    label(ai_content, LV_SYMBOL_BELL, &lv_font_montserrat_12, kPurple);
    label(ai_content, "AI会议助理全程静默记录中", &smartglass_font_12_cjk, kTextDim);
    label(ai_content, "...", &lv_font_montserrat_14, kTextFaint);

    _meeting_status_pill = box(_page_content, compact ? 92 : 118, compact ? 28 : 32,
                               LV_COLOR_MAKE(0x12, 0x12, 0x16), LV_OPA_80, compact ? 14 : 16);
    lv_obj_align(_meeting_status_pill, LV_ALIGN_RIGHT_MID, compact ? -126 : -210, compact ? -6 : -8);
    lv_obj_set_style_border_width(_meeting_status_pill, 1, 0);
    lv_obj_set_style_border_color(_meeting_status_pill, kWhite, 0);
    lv_obj_set_style_border_opa(_meeting_status_pill, LV_OPA_10, 0);
    lv_obj_t *status_dot = box(_meeting_status_pill, 6, 6, kPurple, LV_OPA_COVER, LV_RADIUS_CIRCLE);
    lv_obj_align(status_dot, LV_ALIGN_LEFT_MID, 14, 0);
    _meeting_status_label = label(_meeting_status_pill, "整理中...", &smartglass_font_12_cjk, kTextDim);
    lv_obj_align(_meeting_status_label, LV_ALIGN_LEFT_MID, 28, 0);

    const lv_coord_t card_w = compact ? 214 : 268;
    const lv_coord_t card_h = compact ? 54 : 64;
    const lv_coord_t card_x = _view_width - card_w - (compact ? 22 : 40);
    const lv_coord_t first_y = compact ? 142 : 206;
    const char *card_titles[] = {"待办事项", "排期节点", "关键信息"};
    const char *card_bodies[] = {
        "前端参与后端联调登录页接口",
        "周三前：登录页必须上线",
        "周五下午：交付AR新版视觉初稿",
    };
    const lv_color_t card_colors[] = {kGreen, LV_COLOR_MAKE(0xfb, 0x71, 0x85), kYellow};
    const char *card_icons[] = {LV_SYMBOL_OK, LV_SYMBOL_FILE, LV_SYMBOL_WARNING};
    for(uint8_t i = 0; i < 3; ++i) {
        lv_obj_t *card = box(_page_content, card_w, card_h, LV_COLOR_MAKE(0x07, 0x07, 0x0a), LV_OPA_80, 10);
        _meeting_cards[i] = card;
        lv_obj_set_pos(card, card_x, first_y + i * (card_h + (compact ? 10 : 16)));
        lv_obj_set_style_border_width(card, 1, 0);
        lv_obj_set_style_border_color(card, kWhite, 0);
        lv_obj_set_style_border_opa(card, LV_OPA_10, 0);
        lv_obj_set_style_shadow_width(card, 18, 0);
        lv_obj_set_style_shadow_color(card, kBlack, 0);
        lv_obj_set_style_shadow_opa(card, LV_OPA_50, 0);

        lv_obj_t *icon = label(card, card_icons[i], &lv_font_montserrat_12, card_colors[i]);
        lv_obj_set_pos(icon, 14, compact ? 9 : 10);
        lv_obj_t *card_title = label(card, card_titles[i], &smartglass_font_12_cjk, card_colors[i]);
        lv_obj_set_pos(card_title, 32, compact ? 8 : 9);
        lv_obj_t *body = label(card, card_bodies[i], &smartglass_font_14_cjk, kText);
        lv_obj_set_width(body, card_w - 28);
        lv_label_set_long_mode(body, LV_LABEL_LONG_DOT);
        lv_obj_set_pos(body, 14, compact ? 29 : 34);
    }

    const lv_coord_t transcript_min_width = compact ? 104 : 120;
    _meeting_transcript_pill = box(_page_content, transcript_min_width, compact ? 40 : 42,
                                   LV_COLOR_MAKE(0x05, 0x05, 0x07), LV_OPA_TRANSP, compact ? 20 : 21);
    lv_obj_align(_meeting_transcript_pill, LV_ALIGN_BOTTOM_MID, 0, compact ? -26 : -40);
    lv_obj_set_style_bg_opa(_meeting_transcript_pill, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(_meeting_transcript_pill, 0, 0);
    lv_obj_set_style_border_opa(_meeting_transcript_pill, LV_OPA_TRANSP, 0);
    lv_obj_set_style_outline_width(_meeting_transcript_pill, 0, 0);
    lv_obj_set_style_shadow_width(_meeting_transcript_pill, 0, 0);

    _meeting_transcript_content = lv_obj_create(_meeting_transcript_pill);
    set_plain(_meeting_transcript_content);
    lv_obj_set_size(_meeting_transcript_content, lv_pct(100), 28);
    lv_obj_center(_meeting_transcript_content);
    lv_obj_set_style_bg_opa(_meeting_transcript_content, LV_OPA_TRANSP, 0);
    lv_obj_set_style_pad_column(_meeting_transcript_content, 10, 0);
    lv_obj_set_flex_flow(_meeting_transcript_content, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(_meeting_transcript_content, LV_FLEX_ALIGN_CENTER,
                          LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_clear_flag(_meeting_transcript_content, LV_OBJ_FLAG_CLICKABLE);

    _meeting_mic_icon = lv_img_create(_meeting_transcript_content);
    lv_img_set_src(_meeting_mic_icon, &meeting_microphone_icon);
    lv_obj_set_style_img_recolor_opa(_meeting_mic_icon, LV_OPA_COVER, 0);
    lv_obj_set_style_img_recolor(_meeting_mic_icon, kMeetingMicIdle, 0);
    lv_obj_set_style_img_opa(_meeting_mic_icon, LV_OPA_COVER, 0);

    _meeting_transcript_label = label(_meeting_transcript_content, "", &smartglass_font_14_cjk, kText);
    lv_obj_set_width(_meeting_transcript_label, 1);
    lv_label_set_long_mode(_meeting_transcript_label, LV_LABEL_LONG_WRAP);

    _meeting_step = 0;
    _meeting_line_index = 0;
    _meeting_line_progress = 0;
    _meeting_line_delay = 8;
    _meeting_mic_opacity_step = 0;
    _meeting_transcript_target_width = transcript_min_width;
    updateMeetingPage();
}

void LensReactUI::updateMeetingPage(void)
{
    const char *lines[] = {
        "大家上午好，现在开始同步项目进度。",
        "前端的登录页和侧边栏都完成了没有？",
        "基本完成了，还需要和后端联调接口。",
        "预计周五下午会初稿，重点优化了暗色模式的对比度。",
    };
    constexpr uint8_t line_count = sizeof(lines) / sizeof(lines[0]);
    if(_meeting_line_index >= line_count) {
        _meeting_line_index = line_count - 1;
    }

    if(_meeting_transcript_label) {
        const uint16_t total = utf8_char_count(lines[_meeting_line_index]);
        if(_meeting_line_delay > 0) {
            _meeting_line_delay--;
        } else if(_meeting_line_progress < total) {
            _meeting_line_progress++;
        } else if(_meeting_line_index + 1 < line_count) {
            _meeting_line_index++;
            _meeting_line_progress = 0;
            _meeting_line_delay = 8;
        }
        set_label_utf8_prefix(_meeting_transcript_label, lines[_meeting_line_index], _meeting_line_progress);

        const bool has_voice_input = _meeting_line_delay == 0 && _meeting_line_progress < total;
        if(_meeting_mic_icon) {
            lv_obj_set_style_img_recolor(_meeting_mic_icon,
                                         has_voice_input ? kMeetingMicActive : kMeetingMicIdle, 0);
            if(has_voice_input) {
                _meeting_mic_opacity_step = (_meeting_mic_opacity_step + 1) % 20;
                const uint8_t phase = _meeting_mic_opacity_step <= 10 ? _meeting_mic_opacity_step :
                                                                       20 - _meeting_mic_opacity_step;
                const lv_opa_t opacity = static_cast<lv_opa_t>(140 + phase * 11);
                lv_obj_set_style_img_opa(_meeting_mic_icon, opacity, 0);
            } else {
                _meeting_mic_opacity_step = 0;
                lv_obj_set_style_img_opa(_meeting_mic_icon, LV_OPA_COVER, 0);
            }
        }

        const bool compact = _view_height <= kCompactViewMaxHeight || _view_width <= 700;
        const lv_coord_t min_width = compact ? 104 : 120;
        const lv_coord_t max_width = compact ? 420 : 520;
        const lv_coord_t horizontal_padding = compact ? 16 : 20;
        constexpr lv_coord_t icon_width = 24;
        constexpr lv_coord_t content_gap = 10;
        const char *visible_text = lv_label_get_text(_meeting_transcript_label);
        const lv_coord_t measured_text_width = lv_txt_get_width(
            visible_text, static_cast<uint32_t>(std::strlen(visible_text)), &smartglass_font_14_cjk, 0,
            LV_TEXT_FLAG_NONE);
        const lv_coord_t max_text_width = max_width - horizontal_padding * 2 - icon_width - content_gap;
        const lv_coord_t text_width = LV_MIN(measured_text_width, max_text_width);
        lv_obj_set_width(_meeting_transcript_label, LV_MAX((lv_coord_t)1, text_width));

        lv_point_t text_size = {};
        lv_txt_get_size(&text_size, visible_text, &smartglass_font_14_cjk, 0, 0,
                        LV_MAX((lv_coord_t)1, text_width), LV_TEXT_FLAG_NONE);
        const lv_coord_t content_height = LV_MAX((lv_coord_t)28, text_size.y);
        const lv_coord_t base_height = compact ? 40 : 42;
        const lv_coord_t target_height = LV_MAX(base_height, content_height + (compact ? 10 : 12));
        if(_meeting_transcript_content) {
            lv_obj_set_height(_meeting_transcript_content, content_height);
            lv_obj_center(_meeting_transcript_content);
        }
        if(_meeting_transcript_pill) {
            lv_obj_set_height(_meeting_transcript_pill, target_height);
            lv_obj_align(_meeting_transcript_pill, LV_ALIGN_BOTTOM_MID, 0, compact ? -26 : -40);
        }

        const lv_coord_t target_width = LV_MAX(min_width,
                                                LV_MIN(max_width, icon_width + content_gap + text_width +
                                                                          horizontal_padding * 2));
        if(_meeting_transcript_pill && target_width != _meeting_transcript_target_width) {
            _meeting_transcript_target_width = target_width;
            lv_anim_del(_meeting_transcript_pill, onAnimWidth);
            lv_anim_t resize;
            lv_anim_init(&resize);
            lv_anim_set_var(&resize, _meeting_transcript_pill);
            lv_anim_set_values(&resize, lv_obj_get_width(_meeting_transcript_pill), target_width);
            lv_anim_set_time(&resize, 140);
            lv_anim_set_path_cb(&resize, lv_anim_path_ease_out);
            lv_anim_set_exec_cb(&resize, onAnimWidth);
            lv_anim_start(&resize);
        }
    }

    const bool show_first_card = _meeting_line_index >= 2;
    const bool show_second_card = _meeting_line_index >= 3;
    const bool show_third_card = _meeting_line_index >= 3 && _meeting_line_progress >= 12;
    const bool any_card_visible = show_first_card || show_second_card || show_third_card;

    if(_meeting_status_pill) {
        if(_meeting_line_index >= 1 && !any_card_visible) {
            lv_obj_clear_flag(_meeting_status_pill, LV_OBJ_FLAG_HIDDEN);
            lv_obj_set_style_opa(_meeting_status_pill, LV_OPA_COVER, 0);
        } else {
            lv_obj_add_flag(_meeting_status_pill, LV_OBJ_FLAG_HIDDEN);
        }
    }
    for(uint8_t i = 0; i < 3; ++i) {
        lv_obj_t *card = _meeting_cards[i];
        if(card == nullptr) {
            continue;
        }
        const bool visible = (i == 0 && show_first_card) || (i == 1 && show_second_card) ||
                             (i == 2 && show_third_card);
        if(visible) {
            lv_obj_clear_flag(card, LV_OBJ_FLAG_HIDDEN);
            lv_obj_set_style_opa(card, LV_OPA_COVER, 0);
        } else {
            lv_obj_add_flag(card, LV_OBJ_FLAG_HIDDEN);
        }
    }
}

void LensReactUI::createSettingsPage(void)
{
    if(_view_height <= kCompactViewMaxHeight) {
        lv_obj_t *title = cjk_label(_page_content, "设置", kText);
        lv_obj_align(title, LV_ALIGN_TOP_LEFT, 24, kCompactContentTop);

        const char *names[] = {"显示亮度", "交互方式", "中文字体", "小智唤醒"};
        const char *values[] = {"自动 72%", "触控 + 键盘", "已启用", "全局可用"};
        const lv_color_t colors[] = {kYellow, kBlue, kGreen, kPurple};
        for(uint8_t i = 0; i < 4; ++i) {
            lv_obj_t *row = box(_page_content, _view_width - 48, 46, LV_COLOR_MAKE(0x22, 0x23, 0x27), LV_OPA_COVER, 10);
            lv_obj_align(row, LV_ALIGN_TOP_MID, 0, 68 + i * 52);
            style_soft_row(row);
            lv_obj_t *name = cjk_label(row, names[i], kText);
            lv_obj_align(name, LV_ALIGN_LEFT_MID, 16, 0);
            lv_obj_t *value = cjk_label(row, values[i], colors[i]);
            lv_obj_align(value, LV_ALIGN_RIGHT_MID, -16, 0);
        }
        return;
    }

    lv_obj_t *title = cjk_label(_page_content, "设置", kText);
    lv_obj_align(title, LV_ALIGN_TOP_LEFT, 50, 88);

    struct Setting {
        const char *name;
        const char *value;
        lv_color_t color;
    };
    const Setting settings[] = {
        {"显示亮度", "自动 72%", kYellow},
        {"交互方式", "触控 + 键盘 + 全局 AI", kBlue},
        {"中文字体", "CJK 字体已启用", kGreen},
        {"移植状态", "保留 UTF-8 文案", kCyan},
    };

    lv_obj_t *list = lv_obj_create(_page_content);
    lv_obj_set_size(list, _width - 96, _height - 190);
    lv_obj_align(list, LV_ALIGN_TOP_MID, 0, 148);
    lv_obj_set_style_bg_opa(list, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(list, 0, 0);
    lv_obj_set_style_pad_all(list, 0, 0);
    lv_obj_set_style_pad_row(list, 12, 0);
    lv_obj_set_scrollbar_mode(list, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_flex_flow(list, LV_FLEX_FLOW_COLUMN);

    for(const auto &setting : settings) {
        lv_obj_t *row = box(list, _width - 110, 66, LV_COLOR_MAKE(0x22, 0x23, 0x27), LV_OPA_COVER, 14);
        style_soft_row(row);
        lv_obj_t *name = cjk_label(row, setting.name, kText);
        lv_obj_align(name, LV_ALIGN_LEFT_MID, 22, -10);
        lv_obj_t *value = cjk_label(row, setting.value, setting.color);
        lv_obj_align(value, LV_ALIGN_LEFT_MID, 22, 14);
    }
}

void LensReactUI::createAssistantPage(void)
{
    const bool compact = _view_height <= kCompactViewMaxHeight;
    const lv_coord_t card_width = compact ? (_view_width - 48) : (_view_width - 140);
    const lv_coord_t card_height = compact ? 196 : 270;
    lv_obj_t *card = box(_page_content, card_width, card_height, LV_COLOR_MAKE(0x10, 0x0f, 0x18), LV_OPA_COVER, 18);
    lv_obj_align(card, LV_ALIGN_CENTER, 0, compact ? 10 : -28);
    lv_obj_set_style_border_width(card, 1, 0);
    lv_obj_set_style_border_color(card, kPurple, 0);
    lv_obj_set_style_border_opa(card, LV_OPA_50, 0);

    lv_obj_t *title = cjk_label(card, "小智 AI 助手", kPurple);
    lv_obj_align(title, LV_ALIGN_TOP_LEFT, 18, 16);

    lv_obj_t *prompt = cjk_label(card, "你可以问：导航到会议室，翻译这句话，记录灵感。", kText);
    lv_obj_set_width(prompt, card_width - 36);
    lv_label_set_long_mode(prompt, LV_LABEL_LONG_WRAP);
    lv_obj_align(prompt, LV_ALIGN_TOP_LEFT, 18, compact ? 54 : 62);

    lv_obj_t *answer = cjk_label(card, "示例回复：已准备好，我会在任何页面响应你的唤醒。", kTextDim);
    lv_obj_set_width(answer, card_width - 36);
    lv_label_set_long_mode(answer, LV_LABEL_LONG_WRAP);
    lv_obj_align(answer, LV_ALIGN_BOTTOM_LEFT, 18, -18);
}

void LensReactUI::showAssistantOverlay(void)
{
    if(_viewport == nullptr) {
        return;
    }
    hideAssistantOverlay();
    pausePageTimers();

    _assistant_overlay = box(_viewport, _view_width, _view_height, kBlack, LV_OPA_90, 0);
    lv_obj_center(_assistant_overlay);
    lv_obj_clear_flag(_assistant_overlay, LV_OBJ_FLAG_CLICKABLE);
    if(_status_bar) {
        lv_obj_set_style_text_color(_clock_label, kWhite, 0);
        lv_obj_set_style_text_color(_battery_label, kWhite, 0);
        lv_obj_set_style_bg_color(_battery_fill, kWhite, 0);
        const uint32_t child_count = lv_obj_get_child_cnt(_status_bar);
        for(uint32_t i = 0; i < child_count; ++i) {
            lv_obj_t *child = lv_obj_get_child(_status_bar, i);
            if(child == _assistant_button) {
                continue;
            }
            lv_obj_set_style_border_color(child, kWhite, 0);
            if(lv_obj_get_width(child) <= 4 && lv_obj_get_height(child) <= 10) {
                lv_obj_set_style_bg_color(child, kWhite, 0);
            }
        }
    }

    _assistant_panel = box(_assistant_overlay, _view_width - 64, 230,
                           LV_COLOR_MAKE(0x18, 0x18, 0x1b), LV_OPA_COVER, 24);
    lv_obj_align(_assistant_panel, LV_ALIGN_BOTTOM_MID, 0, -24);
    lv_obj_set_style_bg_grad_color(_assistant_panel, LV_COLOR_MAKE(0x13, 0x19, 0x22), 0);
    lv_obj_set_style_bg_grad_dir(_assistant_panel, LV_GRAD_DIR_HOR, 0);
    lv_obj_set_style_border_width(_assistant_panel, 1, 0);
    lv_obj_set_style_border_color(_assistant_panel, kWhite, 0);
    lv_obj_set_style_border_opa(_assistant_panel, LV_OPA_20, 0);
    lv_obj_set_style_shadow_width(_assistant_panel, 20, 0);
    lv_obj_set_style_shadow_color(_assistant_panel, kBlack, 0);
    lv_obj_set_style_shadow_opa(_assistant_panel, LV_OPA_50, 0);

    lv_obj_t *bot_circle = box(_assistant_panel, 64, 64, LV_COLOR_MAKE(0x63, 0x5b, 0xf6), LV_OPA_COVER,
                               LV_RADIUS_CIRCLE);
    lv_obj_align(bot_circle, LV_ALIGN_TOP_MID, 0, 24);
    lv_obj_set_style_bg_grad_color(bot_circle, LV_COLOR_MAKE(0xa8, 0x55, 0xf7), 0);
    lv_obj_set_style_bg_grad_dir(bot_circle, LV_GRAD_DIR_HOR, 0);
    lv_obj_set_style_shadow_width(bot_circle, 26, 0);
    lv_obj_set_style_shadow_color(bot_circle, kPurple, 0);
    lv_obj_set_style_shadow_opa(bot_circle, LV_OPA_50, 0);
    lv_obj_t *bot = lv_img_create(bot_circle);
    lv_img_set_src(bot, &assistant_bot_icon);
    lv_obj_center(bot);

    lv_obj_t *title = label(_assistant_panel, "我是小智", &smartglass_font_20_cjk, kText);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 102);

    lv_obj_t *body = label(_assistant_panel, "正在聆听：帮我导航到最近的咖啡店", &smartglass_font_14_cjk,
                           kTextDim);
    lv_obj_align(body, LV_ALIGN_TOP_MID, 0, 136);

    constexpr lv_coord_t wave_heights[5] = {10, 18, 24, 18, 12};
    for(uint8_t i = 0; i < 5; ++i) {
        _assistant_wave_bars[i] = box(_assistant_panel, 4, wave_heights[i], kWhite, LV_OPA_80, 2);
        lv_obj_align(_assistant_wave_bars[i], LV_ALIGN_BOTTOM_MID, (static_cast<int>(i) - 2) * 8, -24);
    }
    _assistant_wave_step = 0;
    if(_mic_timer) {
        lv_timer_set_period(_mic_timer, 120);
        lv_timer_resume(_mic_timer);
    }

    lv_obj_align(_assistant_panel, LV_ALIGN_BOTTOM_MID, 0, -24);
    if(_status_bar) {
        lv_obj_move_foreground(_status_bar);
    }
}

void LensReactUI::hideAssistantOverlay(void)
{
    const bool was_visible = _assistant_overlay != nullptr;
    if(_assistant_overlay) {
        lv_obj_del(_assistant_overlay);
        _assistant_overlay = nullptr;
    }
    _assistant_panel = nullptr;
    for(auto &bar : _assistant_wave_bars) {
        bar = nullptr;
    }
    if(_mic_timer) {
        lv_timer_set_period(_mic_timer, 650);
        lv_timer_pause(_mic_timer);
    }
    if(was_visible) {
        resumePageTimer(_current_app);
    }
    if(_status_bar && _current_app < 0) {
        lv_obj_set_style_text_color(_clock_label, kBlack, 0);
        lv_obj_set_style_text_color(_battery_label, kBlack, 0);
        lv_obj_set_style_bg_color(_battery_fill, kBlack, 0);
        const uint32_t child_count = lv_obj_get_child_cnt(_status_bar);
        for(uint32_t i = 0; i < child_count; ++i) {
            lv_obj_t *child = lv_obj_get_child(_status_bar, i);
            if(child == _assistant_button) {
                continue;
            }
            lv_obj_set_style_border_color(child, kBlack, 0);
            if(lv_obj_get_width(child) <= 4 && lv_obj_get_height(child) <= 10) {
                lv_obj_set_style_bg_color(child, kBlack, 0);
            }
        }
    }
}

void LensReactUI::updateClock(void)
{
    if(_clock_label != nullptr) {
        std::tm time_info = {};
        if(fill_local_time(&time_info)) {
            char buf[16];
            std::snprintf(buf, sizeof(buf), "%d:%02d", time_info.tm_hour, time_info.tm_min);
            lv_label_set_text(_clock_label, buf);
        }
    }
    updateSessionTimer();
}

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
    app->_prompt_focus = (app->_prompt_focus + 1) % kPrompterSegmentCount;
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
    const MusicTrack &track = kMusicTracks[app->_music_track_index % kMusicTrackCount];
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

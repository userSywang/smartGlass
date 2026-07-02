#pragma once

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

namespace lens_ui {
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

inline const TileDef kApps[] = {
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

inline lv_color_t color_from_rgb(uint32_t rgb)
{
    const auto red = static_cast<uint8_t>((rgb >> 16) & 0xff);
    const auto green = static_cast<uint8_t>((rgb >> 8) & 0xff);
    const auto blue = static_cast<uint8_t>(rgb & 0xff);
    return LV_COLOR_MAKE(red, green, blue);
}

inline const lv_font_t *font_or_default(const lv_font_t *font)
{
    return font ? font : LV_FONT_DEFAULT;
}

inline void set_plain(lv_obj_t *obj)
{
    lv_obj_remove_style_all(obj);
    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
}

inline lv_obj_t *label(lv_obj_t *parent, const char *text, const lv_font_t *font, lv_color_t color)
{
    lv_obj_t *obj = lv_label_create(parent);
    lv_label_set_text(obj, text);
    lv_obj_set_style_text_font(obj, font_or_default(font), 0);
    lv_obj_set_style_text_color(obj, color, 0);
    return obj;
}

inline lv_obj_t *cjk_label(lv_obj_t *parent, const char *text, lv_color_t color)
{
    return label(parent, text, &smartglass_font_16_cjk, color);
}

inline void format_time(char *buffer, size_t buffer_size, uint16_t seconds)
{
    std::snprintf(buffer, buffer_size, "%u:%02u", static_cast<unsigned>(seconds / 60),
                  static_cast<unsigned>(seconds % 60));
}

inline uint8_t utf8_char_width(unsigned char value)
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

inline uint16_t utf8_char_count(const char *text)
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

inline uint32_t prompter_segment_duration(const char *text)
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

inline void copy_utf8_prefix(char *buffer, size_t buffer_size, const char *text, uint16_t char_count)
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

inline void set_label_utf8_prefix(lv_obj_t *label_obj, const char *text, uint16_t char_count)
{
    if(label_obj == nullptr) {
        return;
    }
    char buffer[256];
    copy_utf8_prefix(buffer, sizeof(buffer), text, char_count);
    lv_label_set_text(label_obj, buffer);
}

inline lv_obj_t *box(lv_obj_t *parent, lv_coord_t w, lv_coord_t h, lv_color_t bg, lv_opa_t opa, lv_coord_t radius)
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

inline lv_obj_t *cover_image(lv_obj_t *parent, const lv_img_dsc_t *source, lv_coord_t target_width,
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

inline lv_obj_t *line(lv_obj_t *parent, const lv_point_t *points, uint16_t count, lv_color_t color, lv_opa_t opa, lv_coord_t width)
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

inline void add_camera_corner(lv_obj_t *parent, lv_align_t align, bool right, bool bottom, lv_coord_t length, lv_coord_t thickness,
                       lv_coord_t inset, lv_color_t color, lv_opa_t opa)
{
    lv_obj_t *corner = box(parent, length, length, kBlack, LV_OPA_TRANSP, 0);
    lv_obj_align(corner, align, right ? -inset : inset, bottom ? -inset : inset);

    lv_obj_t *h = box(corner, length, thickness, color, opa, thickness / 2);
    lv_obj_set_pos(h, 0, bottom ? (length - thickness) : 0);

    lv_obj_t *v = box(corner, thickness, length, color, opa, thickness / 2);
    lv_obj_set_pos(v, right ? (length - thickness) : 0, 0);
}

inline void style_button(lv_obj_t *btn, lv_color_t color, lv_opa_t opa)
{
    lv_obj_set_style_bg_color(btn, color, 0);
    lv_obj_set_style_bg_opa(btn, opa, 0);
    lv_obj_set_style_border_width(btn, 0, 0);
    lv_obj_set_style_shadow_width(btn, 0, 0);
    lv_obj_set_style_radius(btn, LV_RADIUS_CIRCLE, 0);
}

inline void style_no_frame(lv_obj_t *obj)
{
    lv_obj_set_style_bg_opa(obj, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(obj, 0, 0);
    lv_obj_set_style_outline_width(obj, 0, 0);
    lv_obj_set_style_shadow_width(obj, 0, 0);
}

inline void style_soft_row(lv_obj_t *row)
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

inline void style_soft_action(lv_obj_t *obj, lv_color_t accent)
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

inline bool fill_local_time(std::tm *time_info)
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

inline const lv_img_dsc_t *navigation_direction_image(NavigationDirection direction)
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

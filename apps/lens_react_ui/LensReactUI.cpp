#include "LensReactUI.hpp"

#include <cstdio>
#include <ctime>

#if defined(lv_timer_get_user_data)
#define LENS_TIMER_USER_DATA(t) lv_timer_get_user_data(t)
#else
#define LENS_TIMER_USER_DATA(t) ((t)->user_data)
#endif

LV_FONT_DECLARE(smartglass_font_16_cjk);
LV_IMG_DECLARE(nav_bicycle_icon);
LV_IMG_DECLARE(nav_flag_icon);

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
constexpr lv_color_t kHudGreen = LV_COLOR_MAKE(0x31, 0xff, 0x65);
constexpr uint8_t kBatteryPercent = 88;
constexpr lv_coord_t kStatusBarHeight = 30;
constexpr lv_coord_t kPageBottomSafeArea = 0;

struct TileDef {
    const char *id;
    const char *name;
    const char *symbol;
    lv_color_t color;
};

const TileDef kApps[] = {
    {"notes", "备忘", LV_SYMBOL_FILE, kYellow},
    {"camera", "相机", LV_SYMBOL_IMAGE, kText},
    {"nav", "导航", LV_SYMBOL_GPS, kBlue},
    {"music", "音乐", LV_SYMBOL_AUDIO, kRed},
    {"prompter", "提词", LV_SYMBOL_LIST, kGreen},
    {"translate", "翻译", LV_SYMBOL_WIFI, kCyan},
    {"settings", "设置", LV_SYMBOL_SETTINGS, kTextDim},
    {"assistant", "小智", LV_SYMBOL_BELL, kPurple},
};

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
}

LensReactUI::LensReactUI() :
    ESP_Brookesia_PhoneApp("Lens React UI", nullptr, true, false, false)
{
}

bool LensReactUI::init(void)
{
    return true;
}

bool LensReactUI::run(void)
{
    const lv_area_t area = getVisualArea();
    _width = area.x2 - area.x1 + 1;
    _height = area.y2 - area.y1 + 1;
    _selected_index = 2;
    _current_app = -1;
    _prompt_focus = 1;
    _view_width = _width;
    _view_height = _height;
    _item_step = 118;

    _root = lv_obj_create(lv_scr_act());
    set_plain(_root);
    lv_obj_set_size(_root, _width, _height);
    lv_obj_set_pos(_root, area.x1, area.y1);
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
    if(_prompt_timer) {
        lv_timer_pause(_prompt_timer);
    }
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
    return notifyCoreClosed();
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
    return true;
}

void LensReactUI::createHome(lv_coord_t width, lv_coord_t height)
{
    _home = lv_obj_create(_viewport);
    set_plain(_home);
    lv_obj_set_size(_home, width, height);
    lv_obj_set_style_bg_color(_home, kHomeBackground, 0);
    lv_obj_set_style_bg_opa(_home, LV_OPA_COVER, 0);

    _track = lv_obj_create(_home);
    set_plain(_track);
    lv_obj_set_size(_track, (kAppCount - 1) * _item_step + 104, 132);
    lv_obj_align(_track, LV_ALIGN_TOP_MID, 0, kStatusBarHeight + 36);
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

        lv_obj_t *icon_bg = box(shell, 50, 50, kPanel, LV_OPA_COVER, 8);
        lv_obj_center(icon_bg);

        _tile_icons[i] = label(icon_bg, kApps[i].symbol, &lv_font_montserrat_28, kApps[i].color);
        lv_obj_center(_tile_icons[i]);

        _tile_names[i] = cjk_label(tile, kApps[i].name, kText);
        lv_obj_set_width(_tile_names[i], 104);
        lv_label_set_long_mode(_tile_names[i], LV_LABEL_LONG_DOT);
        lv_obj_set_style_text_align(_tile_names[i], LV_TEXT_ALIGN_CENTER, 0);
        lv_obj_align(_tile_names[i], LV_ALIGN_TOP_MID, 0, 94);
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

    _dot_row = lv_obj_create(_status_bar);
    set_plain(_dot_row);
    lv_obj_set_size(_dot_row, 116, 10);
    lv_obj_align(_dot_row, LV_ALIGN_TOP_MID, 0, 11);
    lv_obj_set_style_bg_opa(_dot_row, LV_OPA_TRANSP, 0);

    for(uint8_t i = 0; i < kAppCount; ++i) {
        _dots[i] = box(_dot_row, 4, 4, kTextDim, LV_OPA_COVER, LV_RADIUS_CIRCLE);
        lv_obj_set_pos(_dots[i], 6 + i * 13, 3);
    }

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
    const lv_coord_t content_width = (kAppCount - 1) * _item_step + 104;
    lv_coord_t target_x = (_view_width / 2) - 52 - (lv_coord_t)(_selected_index * _item_step);
    const lv_coord_t min_x = _view_width - content_width - 12;
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
    _current_app = -1;
    if(_home) {
        lv_obj_clear_flag(_home, LV_OBJ_FLAG_HIDDEN);
    }
    if(_page) {
        lv_obj_add_flag(_page, LV_OBJ_FLAG_HIDDEN);
    }
    if(_prompt_timer) {
        lv_timer_pause(_prompt_timer);
    }
    if(_mic_timer) {
        lv_timer_pause(_mic_timer);
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
}

void LensReactUI::showApp(uint8_t index)
{
    if(index >= kAppCount) {
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
    if(_status_bar) {
        lv_obj_move_foreground(_status_bar);
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
        createMusicPage();
        break;
    case 4:
        createPrompterPage();
        break;
    case 5:
        createTranslatePage();
        break;
    case 6:
        createSettingsPage();
        break;
    case 7:
        createAssistantPage();
        break;
    default:
        break;
    }
}

void LensReactUI::clearPageContent(void)
{
    if(_page_content) {
        lv_obj_clean(_page_content);
    }
    for(auto &line : _prompt_lines) {
        line = nullptr;
    }
    _mic_ring = nullptr;
    if(_prompt_timer) {
        lv_timer_pause(_prompt_timer);
    }
    if(_mic_timer) {
        lv_timer_pause(_mic_timer);
    }
}

void LensReactUI::createCameraPage(void)
{
    if(_view_height <= 260) {
        lv_obj_t *frame = box(_page_content, _view_width - 44, _view_height - 58, kBlack, LV_OPA_COVER, 0);
        lv_obj_align(frame, LV_ALIGN_CENTER, 0, -2);

        add_camera_corner(frame, LV_ALIGN_TOP_LEFT, false, false, 24, 2, 0, kText, LV_OPA_COVER);
        add_camera_corner(frame, LV_ALIGN_TOP_RIGHT, true, false, 24, 2, 0, kText, LV_OPA_COVER);
        add_camera_corner(frame, LV_ALIGN_BOTTOM_LEFT, false, true, 24, 2, 0, kText, LV_OPA_COVER);
        add_camera_corner(frame, LV_ALIGN_BOTTOM_RIGHT, true, true, 24, 2, 0, kText, LV_OPA_COVER);

        lv_obj_t *hint = label(_page_content, "Tap photo   Double video", &lv_font_montserrat_14, kText);
        lv_obj_align(hint, LV_ALIGN_BOTTOM_MID, 0, -78);

        return;
    }

    lv_obj_t *frame = box(_page_content, _width - 104, _height - 170, kBlack, LV_OPA_COVER, 0);
    lv_obj_center(frame);

    add_camera_corner(frame, LV_ALIGN_TOP_LEFT, false, false, 44, 3, 0, kText, LV_OPA_COVER);
    add_camera_corner(frame, LV_ALIGN_TOP_RIGHT, true, false, 44, 3, 0, kText, LV_OPA_COVER);
    add_camera_corner(frame, LV_ALIGN_BOTTOM_LEFT, false, true, 44, 3, 0, kText, LV_OPA_COVER);
    add_camera_corner(frame, LV_ALIGN_BOTTOM_RIGHT, true, true, 44, 3, 0, kText, LV_OPA_COVER);

    lv_obj_t *hint = label(_page_content, "Tap photo   Double video", &lv_font_montserrat_16, kText);
    lv_obj_align(hint, LV_ALIGN_BOTTOM_MID, 0, -166);

    lv_obj_t *bar = lv_obj_create(_page_content);
    set_plain(bar);
    lv_obj_set_size(bar, 240, 84);
    lv_obj_align(bar, LV_ALIGN_BOTTOM_MID, 0, -72);
    lv_obj_set_style_bg_opa(bar, LV_OPA_TRANSP, 0);

    lv_obj_t *small = box(bar, 48, 48, kPanel, LV_OPA_COVER, LV_RADIUS_CIRCLE);
    lv_obj_align(small, LV_ALIGN_RIGHT_MID, 0, 0);
    lv_obj_t *cam = label(small, LV_SYMBOL_IMAGE, &lv_font_montserrat_24, kTextDim);
    lv_obj_center(cam);
}

void LensReactUI::createNotesPage(void)
{
    if(_view_height <= 260) {
        lv_obj_t *title = cjk_label(_page_content, "备忘", kText);
        lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 22);
        const char *items[] = {"Team sync at 3 PM", "Buy coffee beans", "UI interaction notes"};
        for(uint8_t i = 0; i < 3; ++i) {
            lv_obj_t *row = box(_page_content, _view_width - 120, 30, LV_COLOR_MAKE(0x22, 0x23, 0x27), LV_OPA_COVER, 10);
            lv_obj_align(row, LV_ALIGN_TOP_MID, 0, 62 + i * 36);
            style_soft_row(row);
            lv_obj_t *text = label(row, items[i], &lv_font_montserrat_14, kText);
            lv_obj_align(text, LV_ALIGN_LEFT_MID, 16, 0);
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
    if(_view_height <= 360) {
        static const lv_point_t arrow_shaft[] = {{12, 40}, {12, 4}};
        static const lv_point_t arrow_head[] = {{12, 4}, {2, 15}, {12, 4}, {22, 15}};

        lv_obj_t *baseline = box(_page_content, _view_width - 24, 2, kHudGreen, LV_OPA_70, 1);
        lv_obj_align(baseline, LV_ALIGN_BOTTOM_MID, 0, -8);

        lv_obj_t *arrow = lv_obj_create(_page_content);
        set_plain(arrow);
        lv_obj_set_size(arrow, 26, 46);
        lv_obj_align(arrow, LV_ALIGN_BOTTOM_LEFT, 20, -18);
        line(arrow, arrow_shaft, 2, kHudGreen, LV_OPA_COVER, 4);
        line(arrow, arrow_head, 4, kHudGreen, LV_OPA_COVER, 4);
        style_no_frame(arrow);

        lv_obj_t *distance = label(_page_content, "724m", &lv_font_montserrat_28, kHudGreen);
        lv_obj_align(distance, LV_ALIGN_BOTTOM_LEFT, 54, -18);
        style_no_frame(distance);

        lv_obj_t *bike = lv_img_create(_page_content);
        lv_img_set_src(bike, &nav_bicycle_icon);
        lv_img_set_zoom(bike, 148);
        style_no_frame(bike);
        lv_obj_align(bike, LV_ALIGN_BOTTOM_MID, -44, -2);

        lv_obj_t *speed = label(_page_content, "5 Km/h", &lv_font_montserrat_14, kHudGreen);
        lv_obj_align(speed, LV_ALIGN_BOTTOM_MID, 18, -16);
        style_no_frame(speed);

        lv_obj_t *remain = cjk_label(_page_content, "剩余:2.8公里 10分钟", kHudGreen);
        lv_obj_align(remain, LV_ALIGN_BOTTOM_RIGHT, -52, -17);
        style_no_frame(remain);

        lv_obj_t *flag = lv_img_create(_page_content);
        lv_img_set_src(flag, &nav_flag_icon);
        lv_img_set_zoom(flag, 132);
        style_no_frame(flag);
        lv_obj_align(flag, LV_ALIGN_BOTTOM_RIGHT, -14, -5);
        return;
    }

    static const lv_point_t arrow_shaft[] = {{18, 62}, {18, 4}};
    static const lv_point_t arrow_head[] = {{18, 4}, {4, 20}, {18, 4}, {32, 20}};

    lv_obj_t *baseline = box(_page_content, _width - 40, 2, kHudGreen, LV_OPA_70, 1);
    lv_obj_align(baseline, LV_ALIGN_BOTTOM_MID, 0, -16);

    lv_obj_t *arrow = lv_obj_create(_page_content);
    set_plain(arrow);
    lv_obj_set_size(arrow, 40, 68);
    lv_obj_align(arrow, LV_ALIGN_BOTTOM_LEFT, 40, -32);
    line(arrow, arrow_shaft, 2, kHudGreen, LV_OPA_COVER, 5);
    line(arrow, arrow_head, 4, kHudGreen, LV_OPA_COVER, 5);
    style_no_frame(arrow);

    lv_obj_t *distance = label(_page_content, "724m", &lv_font_montserrat_38, kHudGreen);
    lv_obj_align(distance, LV_ALIGN_BOTTOM_LEFT, 92, -36);
    style_no_frame(distance);

    lv_obj_t *bike = lv_img_create(_page_content);
    lv_img_set_src(bike, &nav_bicycle_icon);
    lv_img_set_zoom(bike, 170);
    style_no_frame(bike);
    lv_obj_align(bike, LV_ALIGN_BOTTOM_MID, -54, -6);

    lv_obj_t *speed = label(_page_content, "5 Km/h", &lv_font_montserrat_16, kHudGreen);
    lv_obj_align(speed, LV_ALIGN_BOTTOM_MID, 22, -28);
    style_no_frame(speed);

    lv_obj_t *remain = cjk_label(_page_content, "剩余:2.8公里 10分钟", kHudGreen);
    lv_obj_align(remain, LV_ALIGN_BOTTOM_RIGHT, -92, -30);
    style_no_frame(remain);

    lv_obj_t *flag = lv_img_create(_page_content);
    lv_img_set_src(flag, &nav_flag_icon);
    lv_img_set_zoom(flag, 168);
    style_no_frame(flag);
    lv_obj_align(flag, LV_ALIGN_BOTTOM_RIGHT, -34, -12);
}

void LensReactUI::createMusicPage(void)
{
    if(_view_height <= 260) {
        lv_obj_t *cover = box(_page_content, 82, 82, kRed, LV_OPA_COVER, 14);
        lv_obj_align(cover, LV_ALIGN_CENTER, -118, 0);
        lv_obj_set_style_bg_grad_color(cover, kOrange, 0);
        lv_obj_set_style_bg_grad_dir(cover, LV_GRAD_DIR_VER, 0);
        lv_obj_t *music = label(cover, LV_SYMBOL_AUDIO, &lv_font_montserrat_34, LV_COLOR_MAKE(0xff, 0xdf, 0xdf));
        lv_obj_center(music);

        lv_obj_t *song = label(_page_content, "Music Test", &lv_font_montserrat_24, kText);
        lv_obj_align(song, LV_ALIGN_CENTER, 70, -28);
        lv_obj_t *artist = label(_page_content, "Stellar Ocean", &lv_font_montserrat_16, kRed);
        lv_obj_align(artist, LV_ALIGN_CENTER, 70, 4);
        lv_obj_t *controls = label(_page_content, LV_SYMBOL_PREV "   " LV_SYMBOL_PLAY "   " LV_SYMBOL_NEXT, &lv_font_montserrat_24, kTextDim);
        lv_obj_align(controls, LV_ALIGN_CENTER, 70, 40);
        return;
    }

    lv_obj_t *cover = box(_page_content, 250, 250, kRed, LV_OPA_COVER, 28);
    lv_obj_align(cover, LV_ALIGN_CENTER, 0, -92);
    lv_obj_set_style_bg_grad_color(cover, kOrange, 0);
    lv_obj_set_style_bg_grad_dir(cover, LV_GRAD_DIR_VER, 0);
    lv_obj_set_style_shadow_width(cover, 36, 0);
    lv_obj_set_style_shadow_color(cover, LV_COLOR_MAKE(0x7f, 0x1d, 0x1d), 0);

    lv_obj_t *music = label(cover, LV_SYMBOL_AUDIO, &lv_font_montserrat_48, LV_COLOR_MAKE(0xff, 0xdf, 0xdf));
    lv_obj_center(music);

    lv_obj_t *song = label(_page_content, "Stellar Ocean", &lv_font_montserrat_28, kText);
    lv_obj_align(song, LV_ALIGN_CENTER, 0, 76);
    lv_obj_t *artist = label(_page_content, "Smart Glasses Demo Playlist", &lv_font_montserrat_16, kRed);
    lv_obj_align(artist, LV_ALIGN_CENTER, 0, 116);

    lv_obj_t *prev = label(_page_content, LV_SYMBOL_PREV, &lv_font_montserrat_34, kTextDim);
    lv_obj_align(prev, LV_ALIGN_CENTER, -92, 190);
    lv_obj_t *play_btn = box(_page_content, 76, 76, kText, LV_OPA_COVER, LV_RADIUS_CIRCLE);
    lv_obj_align(play_btn, LV_ALIGN_CENTER, 0, 190);
    lv_obj_t *play = label(play_btn, LV_SYMBOL_PLAY, &lv_font_montserrat_34, kBlack);
    lv_obj_center(play);
    lv_obj_t *next = label(_page_content, LV_SYMBOL_NEXT, &lv_font_montserrat_34, kTextDim);
    lv_obj_align(next, LV_ALIGN_CENTER, 92, 190);
}

void LensReactUI::createPrompterPage(void)
{
    if(_view_height <= 260) {
        lv_obj_t *focus = box(_page_content, _view_width - 160, 1, kGreen, LV_OPA_30, 0);
        lv_obj_align(focus, LV_ALIGN_CENTER, 0, 0);
        const char *lines[] = {"Good morning everyone.", "Welcome to the live demo", "of our AI smart glasses", "interaction prototype."};
        for(uint8_t i = 0; i < 4; ++i) {
            _prompt_lines[i] = label(_page_content, lines[i], i == 1 ? &lv_font_montserrat_24 : &lv_font_montserrat_16,
                                     i == 1 ? kGreen : kTextDim);
            lv_obj_align(_prompt_lines[i], LV_ALIGN_CENTER, 0, -50 + i * 30);
        }
        if(_prompt_timer) {
            lv_timer_resume(_prompt_timer);
        }
        return;
    }

    lv_obj_t *focus = box(_page_content, _width, 1, kGreen, LV_OPA_30, 0);
    lv_obj_align(focus, LV_ALIGN_CENTER, 0, 0);

    const char *lines[] = {
        "Good morning everyone.",
        "Welcome to the live demo",
        "of our AI smart glasses",
        "and interaction prototype.",
    };

    for(uint8_t i = 0; i < 4; ++i) {
        _prompt_lines[i] = label(_page_content, lines[i], i == 1 ? &lv_font_montserrat_34 : &lv_font_montserrat_28,
                                 i == 1 ? kGreen : (i == 2 ? kTextDim : kTextFaint));
        lv_obj_align(_prompt_lines[i], LV_ALIGN_CENTER, 0, -78 + i * 52);
    }
    if(_prompt_timer) {
        lv_timer_resume(_prompt_timer);
    }
}

void LensReactUI::createTranslatePage(void)
{
    if(_view_height <= 260) {
        lv_obj_t *top = box(_page_content, _view_width - 130, 58, kBlack, LV_OPA_COVER, 8);
        lv_obj_align(top, LV_ALIGN_TOP_MID, 0, 32);
        lv_obj_set_style_border_width(top, 1, 0);
        lv_obj_set_style_border_color(top, kPanel, 0);
        lv_obj_t *src = label(top, "\"Is it eye tracking or touch?\"", &lv_font_montserrat_16, kTextDim);
        lv_obj_center(src);

        lv_obj_t *bottom = box(_page_content, _view_width - 130, 58, LV_COLOR_MAKE(0x09, 0x09, 0x0b), LV_OPA_COVER, 8);
        lv_obj_align(bottom, LV_ALIGN_BOTTOM_MID, 0, -24);
        lv_obj_set_style_border_width(bottom, 1, 0);
        lv_obj_set_style_border_color(bottom, kCyan, 0);
        lv_obj_t *out = label(bottom, "Translate Test: English output", &lv_font_montserrat_16, kCyan);
        lv_obj_center(out);
        return;
    }

    lv_obj_t *top = box(_page_content, _width, _height / 2, kBlack, LV_OPA_COVER, 0);
    lv_obj_align(top, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_set_style_border_side(top, LV_BORDER_SIDE_BOTTOM, 0);
    lv_obj_set_style_border_width(top, 1, 0);
    lv_obj_set_style_border_color(top, kPanel, 0);

    lv_obj_t *tag_en = box(top, 86, 28, kPanel, LV_OPA_COVER, 14);
    lv_obj_align(tag_en, LV_ALIGN_TOP_RIGHT, -40, 76);
    lv_obj_t *en = label(tag_en, "English", &lv_font_montserrat_14, kTextFaint);
    lv_obj_center(en);

    lv_obj_t *en_text = label(top, "\"Wow, this smart glass interface looks amazing. Is it controlled by eye tracking or touch?\"",
                              &lv_font_montserrat_24, kTextDim);
    lv_obj_set_width(en_text, _width - 130);
    lv_label_set_long_mode(en_text, LV_LABEL_LONG_WRAP);
    lv_obj_set_style_text_align(en_text, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_center(en_text);

    lv_obj_t *bottom = box(_page_content, _width, _height / 2, LV_COLOR_MAKE(0x09, 0x09, 0x0b), LV_OPA_COVER, 0);
    lv_obj_align(bottom, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_t *tag_cn = box(bottom, 104, 30, LV_COLOR_MAKE(0x08, 0x30, 0x38), LV_OPA_70, 15);
    lv_obj_align(tag_cn, LV_ALIGN_TOP_LEFT, 40, 28);
    lv_obj_t *cn = label(tag_cn, "Output  " LV_SYMBOL_AUDIO, &lv_font_montserrat_14, kCyan);
    lv_obj_center(cn);

    lv_obj_t *cn_text = label(bottom, "\"This interface looks great. Is it controlled by eye tracking or touch?\"",
                              &lv_font_montserrat_28, kCyan);
    lv_obj_set_width(cn_text, _width - 120);
    lv_label_set_long_mode(cn_text, LV_LABEL_LONG_WRAP);
    lv_obj_set_style_text_align(cn_text, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_center(cn_text);

    _mic_ring = box(bottom, 78, 78, kCyan, LV_OPA_20, LV_RADIUS_CIRCLE);
    lv_obj_align(_mic_ring, LV_ALIGN_BOTTOM_MID, 0, -70);
    lv_obj_set_style_border_width(_mic_ring, 1, 0);
    lv_obj_set_style_border_color(_mic_ring, kCyan, 0);
    lv_obj_t *mic = label(_mic_ring, LV_SYMBOL_AUDIO, &lv_font_montserrat_28, kCyan);
    lv_obj_center(mic);
    if(_mic_timer) {
        lv_timer_resume(_mic_timer);
    }
}

void LensReactUI::createSettingsPage(void)
{
    if(_view_height <= 260) {
        lv_obj_t *title = cjk_label(_page_content, "设置", kText);
        lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 22);

        const char *items[] = {"亮度 自动", "中文字体 已启用", "小智唤醒 全局可用"};
        for(uint8_t i = 0; i < 3; ++i) {
            lv_obj_t *row = box(_page_content, _view_width - 140, 30, LV_COLOR_MAKE(0x22, 0x23, 0x27), LV_OPA_COVER, 10);
            lv_obj_align(row, LV_ALIGN_TOP_MID, 0, 60 + i * 36);
            style_soft_row(row);
            lv_obj_t *text = cjk_label(row, items[i], i == 2 ? kPurple : kText);
            lv_obj_align(text, LV_ALIGN_LEFT_MID, 16, 0);
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
        {"中文字体", "SimSun CJK 已启用", kGreen},
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
    const lv_coord_t card_width = _view_height <= 260 ? (_view_width - 84) : (_width - 140);
    const lv_coord_t card_height = _view_height <= 260 ? 118 : 270;
    lv_obj_t *card = box(_page_content, card_width, card_height, LV_COLOR_MAKE(0x10, 0x0f, 0x18), LV_OPA_COVER, 18);
    lv_obj_align(card, LV_ALIGN_CENTER, 0, _view_height <= 260 ? -2 : -28);
    lv_obj_set_style_border_width(card, 1, 0);
    lv_obj_set_style_border_color(card, kPurple, 0);
    lv_obj_set_style_border_opa(card, LV_OPA_50, 0);

    lv_obj_t *title = cjk_label(card, "小智 AI 助手", kPurple);
    lv_obj_align(title, LV_ALIGN_TOP_LEFT, 18, 16);

    lv_obj_t *prompt = cjk_label(card, "你可以问：导航到会议室，翻译这句话，记录灵感。", kText);
    lv_obj_set_width(prompt, card_width - 36);
    lv_label_set_long_mode(prompt, LV_LABEL_LONG_WRAP);
    lv_obj_align(prompt, LV_ALIGN_TOP_LEFT, 18, _view_height <= 260 ? 46 : 62);

    lv_obj_t *answer = cjk_label(card, "示例回复：已准备好，我会在任何页面响应你的呼出。", kTextDim);
    lv_obj_set_width(answer, card_width - 36);
    lv_label_set_long_mode(answer, LV_LABEL_LONG_WRAP);
    lv_obj_align(answer, LV_ALIGN_BOTTOM_LEFT, 18, -18);
}

void LensReactUI::showAssistantOverlay(void)
{
    if(_root == nullptr) {
        return;
    }
    hideAssistantOverlay();

    _assistant_overlay = box(_root, _view_width, _view_height, kBlack, LV_OPA_70, 0);
    lv_obj_center(_assistant_overlay);
    lv_obj_add_flag(_assistant_overlay, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(_assistant_overlay, onOverlayDismissed, LV_EVENT_CLICKED, this);

    lv_obj_t *panel = box(_assistant_overlay, LV_MIN((lv_coord_t)520, (lv_coord_t)(_view_width - 54)),
                          LV_MIN((lv_coord_t)150, (lv_coord_t)(_view_height - 32)),
                          LV_COLOR_MAKE(0x10, 0x0f, 0x18), LV_OPA_COVER, 18);
    lv_obj_center(panel);
    lv_obj_set_style_border_width(panel, 1, 0);
    lv_obj_set_style_border_color(panel, kPurple, 0);

    lv_obj_t *title = cjk_label(panel, "小智正在聆听", kPurple);
    lv_obj_align(title, LV_ALIGN_TOP_LEFT, 20, 18);
    lv_obj_t *body = cjk_label(panel, "全局呼出成功。试试说：打开设置、开始翻译、记录会议重点。", kText);
    lv_obj_set_width(body, lv_obj_get_width(panel) - 40);
    lv_label_set_long_mode(body, LV_LABEL_LONG_WRAP);
    lv_obj_align(body, LV_ALIGN_TOP_LEFT, 20, 54);
    lv_obj_t *hint = cjk_label(panel, "点击任意位置关闭", kTextFaint);
    lv_obj_align(hint, LV_ALIGN_BOTTOM_RIGHT, -18, -14);
}

void LensReactUI::hideAssistantOverlay(void)
{
    if(_assistant_overlay) {
        lv_obj_del(_assistant_overlay);
        _assistant_overlay = nullptr;
    }
}

void LensReactUI::updateClock(void)
{
    if(_clock_label == nullptr) {
        return;
    }
    std::tm time_info = {};
    if(!fill_local_time(&time_info)) {
        return;
    }
    char buf[16];
    std::snprintf(buf, sizeof(buf), "%d:%02d", time_info.tm_hour, time_info.tm_min);
    lv_label_set_text(_clock_label, buf);
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
    if(app == nullptr || app->_current_app != 4) {
        return;
    }
    app->_prompt_focus = (app->_prompt_focus + 1) % 4;
    for(uint8_t i = 0; i < 4; ++i) {
        if(app->_prompt_lines[i] == nullptr) {
            continue;
        }
        const bool active = i == app->_prompt_focus;
        const bool compact = app->_view_height <= 260;
        lv_obj_set_style_text_font(app->_prompt_lines[i],
                                   active ? (compact ? &lv_font_montserrat_24 : &lv_font_montserrat_34) :
                                            (compact ? &lv_font_montserrat_16 : &lv_font_montserrat_28),
                                   0);
        lv_obj_set_style_text_color(app->_prompt_lines[i], active ? kGreen : (i == ((app->_prompt_focus + 1) % 4) ? kTextDim : kTextFaint), 0);
        lv_obj_align(app->_prompt_lines[i], LV_ALIGN_CENTER, 0,
                     compact ? (-50 + ((int)i - (int)app->_prompt_focus + 1) * 30) :
                               (-78 + ((int)i - (int)app->_prompt_focus + 1) * 52));
    }
}

void LensReactUI::onMicTimer(lv_timer_t *timer)
{
    auto *app = static_cast<LensReactUI *>(timer ? LENS_TIMER_USER_DATA(timer) : nullptr);
    if(app == nullptr || app->_mic_ring == nullptr) {
        return;
    }
    app->_mic_expand = !app->_mic_expand;
    lv_obj_set_size(app->_mic_ring, app->_mic_expand ? 88 : 70, app->_mic_expand ? 88 : 70);
    lv_obj_set_style_bg_opa(app->_mic_ring, app->_mic_expand ? LV_OPA_10 : LV_OPA_30, 0);
    lv_obj_align(app->_mic_ring, LV_ALIGN_BOTTOM_MID, 0, -70);
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
    if(key == 'a' || key == 'A') {
        app->showAssistantOverlay();
        return;
    }
    if(app->_assistant_overlay) {
        if(key == LV_KEY_ESC || key == LV_KEY_BACKSPACE || key == LV_KEY_ENTER) {
            app->hideAssistantOverlay();
        }
        return;
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

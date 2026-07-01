#include "LensUIInternal.hpp"

using namespace lens_ui;

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

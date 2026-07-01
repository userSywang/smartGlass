#include "LensUIInternal.hpp"

using namespace lens_ui;

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

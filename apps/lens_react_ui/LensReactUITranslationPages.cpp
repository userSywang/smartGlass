#include "LensUIInternal.hpp"

using namespace lens_ui;

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

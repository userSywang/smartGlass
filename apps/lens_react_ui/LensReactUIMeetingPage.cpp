#include "LensUIInternal.hpp"

using namespace lens_ui;

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
    const LensDataView<MeetingCardSample> cards = _data_provider.meetingCards();
    const std::size_t card_count = LV_MIN(cards.size, static_cast<std::size_t>(3));
    for(std::size_t i = 0; i < card_count; ++i) {
        const MeetingCardSample &sample = cards[i];
        lv_color_t card_color = kGreen;
        const char *card_icon = LV_SYMBOL_OK;
        if(sample.kind == MeetingCardKind::Schedule) {
            card_color = LV_COLOR_MAKE(0xfb, 0x71, 0x85);
            card_icon = LV_SYMBOL_FILE;
        } else if(sample.kind == MeetingCardKind::KeyInfo) {
            card_color = kYellow;
            card_icon = LV_SYMBOL_WARNING;
        }
        lv_obj_t *card = box(_page_content, card_w, card_h, LV_COLOR_MAKE(0x07, 0x07, 0x0a), LV_OPA_80, 10);
        _meeting_cards[i] = card;
        lv_obj_set_pos(card, card_x, first_y + i * (card_h + (compact ? 10 : 16)));
        lv_obj_set_style_border_width(card, 1, 0);
        lv_obj_set_style_border_color(card, kWhite, 0);
        lv_obj_set_style_border_opa(card, LV_OPA_10, 0);
        lv_obj_set_style_shadow_width(card, 18, 0);
        lv_obj_set_style_shadow_color(card, kBlack, 0);
        lv_obj_set_style_shadow_opa(card, LV_OPA_50, 0);

        lv_obj_t *icon = label(card, card_icon, &lv_font_montserrat_12, card_color);
        lv_obj_set_pos(icon, 14, compact ? 9 : 10);
        lv_obj_t *card_title = label(card, sample.title, &smartglass_font_12_cjk, card_color);
        lv_obj_set_pos(card_title, 32, compact ? 8 : 9);
        lv_obj_t *body = label(card, sample.body, &smartglass_font_14_cjk, kText);
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
    const LensDataView<const char *> lines = _data_provider.meetingTranscriptLines();
    if(lines.size == 0) {
        return;
    }
    const std::size_t line_count = lines.size;
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

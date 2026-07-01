#include "LensUIInternal.hpp"

using namespace lens_ui;

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
    const LensDataView<NotificationSample> samples = _data_provider.notifications();
    if(type >= samples.size) {
        return;
    }

    const NotificationSample &sample = samples[type];
    const char *icon = LV_SYMBOL_BELL;
    lv_color_t accent = kGreen;
    if(sample.channel == NotificationChannel::Sms) {
        icon = LV_SYMBOL_CALL;
        accent = kBlue;
    } else if(sample.channel == NotificationChannel::DingTalk) {
        icon = LV_SYMBOL_LIST;
        accent = kOrange;
    }
    addNotificationBubble(sample.source, sample.sender, sample.message, icon, accent);
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

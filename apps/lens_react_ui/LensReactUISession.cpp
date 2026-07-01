#include "LensUIInternal.hpp"

using namespace lens_ui;

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

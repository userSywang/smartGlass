#include "LensUIInternal.hpp"

using namespace lens_ui;

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

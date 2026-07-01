#include "LensUIInternal.hpp"

using namespace lens_ui;

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

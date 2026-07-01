#include "LensUIInternal.hpp"

using namespace lens_ui;

void LensReactUI::createPrompterPage(void)
{
    const bool compact = _view_height <= kCompactViewMaxHeight || _view_width <= 700;

    createSessionTimerPill(LV_ALIGN_TOP_LEFT, compact ? 16 : 24, compact ? 38 : 48);

    _prompt_cursor = box(_page_content, 3, compact ? 16 : 20, kGreen, LV_OPA_COVER, 2);
    lv_obj_align(_prompt_cursor, LV_ALIGN_TOP_LEFT, compact ? 18 : 24, compact ? 76 : 90);

    _prompt_text_label = label(_page_content, "", compact ? &smartglass_font_14_cjk : &smartglass_font_16_cjk,
                               kText);
    lv_obj_set_width(_prompt_text_label, compact ? (_view_width - 56) : LV_MIN((lv_coord_t)944,
                                                                                (lv_coord_t)(_view_width - 80)));
    lv_label_set_long_mode(_prompt_text_label, LV_LABEL_LONG_WRAP);
    lv_label_set_recolor(_prompt_text_label, true);
    lv_obj_set_style_text_align(_prompt_text_label, LV_TEXT_ALIGN_LEFT, 0);
    lv_obj_set_style_text_line_space(_prompt_text_label, compact ? 8 : 12, 0);
    lv_obj_align(_prompt_text_label, LV_ALIGN_CENTER, 0, compact ? 20 : 28);

    _prompt_focus = 0;
    updatePrompterPage();
}

void LensReactUI::updatePrompterPage(void)
{
    if(_prompt_text_label == nullptr) {
        return;
    }

    char text[1536] = {};
    size_t used = 0;
    for(uint8_t i = 0; i < kPrompterSegmentCount; ++i) {
        const char *segment = kPrompterSegments[i];
        const int written = i == _prompt_focus
                                ? std::snprintf(text + used, sizeof(text) - used, "#34d399 %s#", segment)
                                : std::snprintf(text + used, sizeof(text) - used, "%s", segment);
        if(written < 0 || static_cast<size_t>(written) >= sizeof(text) - used) {
            break;
        }
        used += static_cast<size_t>(written);
    }
    lv_label_set_text(_prompt_text_label, text);
    if(_prompt_timer) {
        lv_timer_set_period(_prompt_timer, prompter_segment_duration(kPrompterSegments[_prompt_focus]));
    }
}

#pragma once

#include <cstdint>

#include "lvgl.h"
#include "esp_brookesia.hpp"

class LensReactUI : public ESP_Brookesia_PhoneApp {
public:
    LensReactUI();
    ~LensReactUI() override = default;

    bool init(void) override;
    bool run(void) override;
    bool back(void) override;
    bool close(void) override;

private:
    static constexpr uint8_t kAppCount = 8;

    static void onClockTimer(lv_timer_t *timer);
    static void onPromptTimer(lv_timer_t *timer);
    static void onMicTimer(lv_timer_t *timer);
    static void onRootPressed(lv_event_t *event);
    static void onRootReleased(lv_event_t *event);
    static void onRootKey(lv_event_t *event);
    static void onTileClicked(lv_event_t *event);
    static void onAssistantClicked(lv_event_t *event);
    static void onOverlayDismissed(lv_event_t *event);
    static void onAnimX(void *obj, int32_t value);

    void createHome(lv_coord_t width, lv_coord_t height);
    void createStatusBar(lv_coord_t width, lv_coord_t height);
    void createPageHost(lv_coord_t width, lv_coord_t height);
    void rebuildPage(void);
    void showHome(void);
    void showApp(uint8_t index);
    void showAssistantOverlay(void);
    void hideAssistantOverlay(void);
    void selectIndex(uint8_t index, bool animate);
    void updateTileStyles(void);
    void updateClock(void);
    void updateDots(void);
    void clearPageContent(void);

    void createCameraPage(void);
    void createNotesPage(void);
    void createNavigationPage(void);
    void createMusicPage(void);
    void createPrompterPage(void);
    void createTranslatePage(void);
    void createSettingsPage(void);
    void createAssistantPage(void);

    lv_obj_t *_root = nullptr;
    lv_obj_t *_viewport = nullptr;
    lv_obj_t *_home = nullptr;
    lv_obj_t *_track = nullptr;
    lv_obj_t *_status_bar = nullptr;
    lv_obj_t *_dot_row = nullptr;
    lv_obj_t *_assistant_button = nullptr;
    lv_obj_t *_tiles[kAppCount] = {};
    lv_obj_t *_tile_icons[kAppCount] = {};
    lv_obj_t *_tile_names[kAppCount] = {};
    lv_obj_t *_dots[kAppCount] = {};
    lv_obj_t *_page = nullptr;
    lv_obj_t *_page_content = nullptr;
    lv_obj_t *_clock_label = nullptr;
    lv_obj_t *_battery_label = nullptr;
    lv_obj_t *_battery_fill = nullptr;
    lv_obj_t *_assistant_overlay = nullptr;
    lv_obj_t *_prompt_lines[4] = {};
    lv_obj_t *_mic_ring = nullptr;

    lv_timer_t *_clock_timer = nullptr;
    lv_timer_t *_prompt_timer = nullptr;
    lv_timer_t *_mic_timer = nullptr;

    lv_coord_t _width = 0;
    lv_coord_t _height = 0;
    lv_coord_t _view_width = 640;
    lv_coord_t _view_height = 200;
    lv_coord_t _item_step = 128;
    lv_coord_t _press_x = 0;
    uint8_t _selected_index = 1;
    int8_t _current_app = -1;
    uint8_t _prompt_focus = 1;
    bool _dragging = false;
    bool _suppress_click = false;
    bool _mic_expand = false;
};

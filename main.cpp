#include "lvgl/lvgl.h"
#include "lv_drivers/display/fbdev.h"
#include "lv_drivers/indev/evdev.h"

#include <SDL2/SDL.h>
#include "lv_drivers/sdl/sdl.h"

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <sys/time.h>
#include <unistd.h>

#include "esp_brookesia.hpp"
#include "apps/lens_react_ui/LensReactUI.hpp"

#define MONITOR_HOR_RES SDL_HOR_RES
#define MONITOR_VER_RES SDL_VER_RES

#define BSP_LCD_H_RES MONITOR_HOR_RES
#define BSP_LCD_V_RES MONITOR_VER_RES

static void hal_init(void);
static void smart_glass_init(void);

int SDL_main(int argc, char *argv[])
{
    LV_UNUSED(argc);
    LV_UNUSED(argv);

    ESP_BROOKESIA_LOGI("Starting smartGlass");

    lv_init();
    hal_init();
    smart_glass_init();

    while(1) {
        lv_timer_handler();
        usleep(5000);
    }

    return 0;
}

extern "C" {

uint32_t custom_tick_get(void)
{
    static uint64_t start_ms = 0;
    if(start_ms == 0) {
        struct timeval tv_start;
        gettimeofday(&tv_start, nullptr);
        start_ms = (tv_start.tv_sec * 1000000 + tv_start.tv_usec) / 1000;
    }

    struct timeval tv_now;
    gettimeofday(&tv_now, nullptr);
    const uint64_t now_ms = (tv_now.tv_sec * 1000000 + tv_now.tv_usec) / 1000;

    return (uint32_t)(now_ms - start_ms);
}

}

static void smart_glass_init(void)
{
    ESP_BROOKESIA_LOGI("Initialize Brookesia minimal phone shell");

    ESP_Brookesia_Phone *phone = new ESP_Brookesia_Phone();
    ESP_BROOKESIA_CHECK_NULL_EXIT(phone, "Create phone failed");

    ESP_Brookesia_PhoneStylesheet_t *stylesheet = nullptr;
    if((BSP_LCD_H_RES == 1024) && (BSP_LCD_V_RES == 600)) {
        stylesheet = new ESP_Brookesia_PhoneStylesheet_t ESP_BROOKESIA_PHONE_1024_600_DARK_STYLESHEET();
        ESP_BROOKESIA_CHECK_NULL_EXIT(stylesheet, "Create 1024x600 stylesheet failed");
    } else if((BSP_LCD_H_RES == 1280) && (BSP_LCD_V_RES == 800)) {
        stylesheet = new ESP_Brookesia_PhoneStylesheet_t ESP_BROOKESIA_PHONE_1280_800_DARK_STYLESHEET();
        ESP_BROOKESIA_CHECK_NULL_EXIT(stylesheet, "Create 1280x800 stylesheet failed");
    } else if((BSP_LCD_H_RES == 800) && (BSP_LCD_V_RES == 1280)) {
        stylesheet = new ESP_Brookesia_PhoneStylesheet_t ESP_BROOKESIA_PHONE_800_1280_DARK_STYLESHEET();
        ESP_BROOKESIA_CHECK_NULL_EXIT(stylesheet, "Create 800x1280 stylesheet failed");
    }

    if(stylesheet != nullptr) {
        ESP_BROOKESIA_LOGI("Using stylesheet: %s", stylesheet->core.name);
        ESP_BROOKESIA_CHECK_FALSE_EXIT(phone->addStylesheet(stylesheet), "Add stylesheet failed");
        ESP_BROOKESIA_CHECK_FALSE_EXIT(phone->activateStylesheet(stylesheet), "Activate stylesheet failed");
        delete stylesheet;
    }

    ESP_BROOKESIA_CHECK_FALSE_EXIT(phone->begin(), "Begin phone failed");

    LensReactUI *lens_react_ui = new LensReactUI();
    ESP_BROOKESIA_CHECK_NULL_EXIT(lens_react_ui, "Create LensReactUI failed");

    const int app_id = phone->installApp(lens_react_ui);
    ESP_BROOKESIA_CHECK_FALSE_EXIT(app_id >= 0, "Install LensReactUI failed");

    ESP_Brookesia_CoreAppEventData_t event_data = {
        .id = app_id,
        .type = ESP_BROOKESIA_CORE_APP_EVENT_TYPE_START,
        .data = nullptr,
    };
    ESP_BROOKESIA_CHECK_FALSE_EXIT(phone->sendAppEvent(&event_data), "Start LensReactUI failed");

    ESP_BROOKESIA_LOGI("LensReactUI started as app id %d", app_id);
}

static void hal_init(void)
{
    static lv_indev_drv_t mouse_drv;
    static lv_indev_drv_t keyboard_drv;
    static lv_indev_drv_t mousewheel_drv;

    lv_indev_drv_init(&mouse_drv);
    mouse_drv.type = LV_INDEV_TYPE_POINTER;

    lv_indev_drv_init(&keyboard_drv);
    keyboard_drv.type = LV_INDEV_TYPE_KEYPAD;

    lv_indev_drv_init(&mousewheel_drv);
    mousewheel_drv.type = LV_INDEV_TYPE_ENCODER;

    lv_group_t *group = lv_group_create();
    lv_group_set_default(group);

    ESP_BROOKESIA_LOGI("Initialize SDL display");
    sdl_init();

    static lv_disp_draw_buf_t draw_buf;
    static lv_color_t buf1[MONITOR_HOR_RES * 100];
    static lv_color_t buf2[MONITOR_HOR_RES * 100];
    lv_disp_draw_buf_init(&draw_buf, buf1, buf2, MONITOR_HOR_RES * 100);

    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.draw_buf = &draw_buf;
    disp_drv.flush_cb = sdl_display_flush;
    disp_drv.hor_res = MONITOR_HOR_RES;
    disp_drv.ver_res = MONITOR_VER_RES;
    disp_drv.antialiasing = 1;

    lv_disp_t *disp = lv_disp_drv_register(&disp_drv);
    ESP_BROOKESIA_CHECK_NULL_EXIT(disp, "Register LVGL display failed");

    mouse_drv.read_cb = sdl_mouse_read;
    keyboard_drv.read_cb = sdl_keyboard_read;
    mousewheel_drv.read_cb = sdl_mousewheel_read;

    lv_theme_t *theme = lv_theme_default_init(
        disp,
        lv_palette_main(LV_PALETTE_BLUE),
        lv_palette_main(LV_PALETTE_RED),
        LV_THEME_DEFAULT_DARK,
        LV_FONT_DEFAULT
    );
    lv_disp_set_theme(disp, theme);

    lv_indev_t *mouse = lv_indev_drv_register(&mouse_drv);
    lv_indev_t *keyboard = lv_indev_drv_register(&keyboard_drv);
    lv_indev_t *mousewheel = lv_indev_drv_register(&mousewheel_drv);

    lv_indev_set_group(keyboard, group);
    lv_indev_set_group(mousewheel, group);

    LV_IMG_DECLARE(mouse_cursor_icon);
    lv_obj_t *cursor = lv_img_create(lv_scr_act());
    lv_img_set_src(cursor, &mouse_cursor_icon);
    lv_indev_set_cursor(mouse, cursor);
}

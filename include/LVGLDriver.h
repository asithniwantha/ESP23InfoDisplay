#ifndef LVGL_DRIVER_H
#define LVGL_DRIVER_H

#include <lvgl.h>
#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>
#include "Defines.h"

class LVGLDriver {
public:
    LVGLDriver();
    void init();
    void tick();
    
private:
    TFT_eSPI tft;
    XPT2046_Touchscreen ts;
    
    static lv_disp_draw_buf_t draw_buf;
    static lv_color_t buf[SCREEN_WIDTH * 10];
    static lv_disp_drv_t disp_drv;
    static lv_indev_drv_t indev_drv;
    
    static void disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);
    static void touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data);
};

#endif // LVGL_DRIVER_H

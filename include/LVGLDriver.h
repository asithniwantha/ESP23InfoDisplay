#pragma once
#include <TFT_eSPI.h>
#include "lvgl.h"

class LVGLDriver {
public:
    LVGLDriver();
    void init();
    void tick();

private:
    TFT_eSPI tft;
    static void disp_flush_cb(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p);
};
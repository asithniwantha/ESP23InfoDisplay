#include "LVGLDriver.h"
#include "Defines.h"

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf1[LVGL_BUFFER_SIZE];

LVGLDriver::LVGLDriver() : tft(TFT_eSPI()) {}

void LVGLDriver::init() {
    tft.begin();
    tft.setRotation(1);
    tft.invertDisplay( true );

    lv_init();
    lv_disp_draw_buf_init(&draw_buf, buf1, NULL, LVGL_BUFFER_SIZE);

    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = TFT_WIDTH;
    disp_drv.ver_res = TFT_HEIGHT;
    disp_drv.flush_cb = disp_flush_cb;
    disp_drv.draw_buf = &draw_buf;
    disp_drv.user_data = this;
    lv_disp_drv_register(&disp_drv);
}

void LVGLDriver::tick() {
    lv_timer_handler();
}

void LVGLDriver::disp_flush_cb(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p) {
    LVGLDriver* driver = (LVGLDriver*)disp_drv->user_data;
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);
    driver->tft.startWrite();
    driver->tft.setAddrWindow(area->x1, area->y1, w, h);
    driver->tft.pushColors((uint16_t*)color_p, w * h, true);
    driver->tft.endWrite();
    lv_disp_flush_ready(disp_drv);
}

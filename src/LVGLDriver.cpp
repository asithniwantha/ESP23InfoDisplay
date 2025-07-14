#include "LVGLDriver.h"

// Static member definitions
lv_disp_draw_buf_t LVGLDriver::draw_buf;
lv_color_t LVGLDriver::buf[SCREEN_WIDTH * 10];
lv_disp_drv_t LVGLDriver::disp_drv;
lv_indev_drv_t LVGLDriver::indev_drv;

LVGLDriver::LVGLDriver() : tft(TFT_eSPI()), ts(XPT2046_CS) {}

void LVGLDriver::init() {
    // Initialize TFT
    tft.init();
    tft.setRotation(1);
    tft.invertDisplay( true );
    tft.fillScreen(TFT_BLACK);
    
    // Initialize touch
    ts.begin();
    ts.setRotation(1);
    
    // Initialize LVGL
    lv_init();
    
    // Initialize the display buffer
    lv_disp_draw_buf_init(&draw_buf, buf, NULL, SCREEN_WIDTH * 10);
    
    // Initialize the display driver
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = SCREEN_WIDTH;
    disp_drv.ver_res = SCREEN_HEIGHT;
    disp_drv.flush_cb = disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);
    
    // Initialize the input device driver
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = touchpad_read;
    lv_indev_drv_register(&indev_drv);
}

void LVGLDriver::tick() {
    lv_timer_handler();
}

void LVGLDriver::disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);
    
    TFT_eSPI* tft_instance = (TFT_eSPI*)disp->user_data;
    if (!tft_instance) {
        // Use global instance if user_data is not set
        static TFT_eSPI global_tft;
        tft_instance = &global_tft;
    }
    
    tft_instance->startWrite();
    tft_instance->setAddrWindow(area->x1, area->y1, w, h);
    tft_instance->pushColors((uint16_t*)&color_p->full, w * h, true);
    tft_instance->endWrite();
    
    lv_disp_flush_ready(disp);
}

void LVGLDriver::touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data) {
    static XPT2046_Touchscreen ts_static(XPT2046_CS);
    static bool ts_initialized = false;
    
    if (!ts_initialized) {
        ts_static.begin();
        ts_static.setRotation(1);
        ts_initialized = true;
    }
    
    if (ts_static.touched()) {
        TS_Point p = ts_static.getPoint();
        
        // Map touch coordinates to screen coordinates
        data->point.x = map(p.x, 200, 3700, 1, SCREEN_WIDTH);
        data->point.y = map(p.y, 240, 3800, 1, SCREEN_HEIGHT);
        data->state = LV_INDEV_STATE_PR;
    } else {
        data->state = LV_INDEV_STATE_REL;
    }
}

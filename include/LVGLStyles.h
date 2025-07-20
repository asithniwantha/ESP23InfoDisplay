#ifndef LVGL_STYLES_H
#define LVGL_STYLES_H

#include <lvgl.h>
#include "UIConfig.h"

class LVGLStyles {
public:
    LVGLStyles();
    ~LVGLStyles();
    
    void init();
    void cleanup();
    
    // Getters for styles
    lv_style_t* getCardStyle() { return &style_card; }
    lv_style_t* getProgressStyle() { return &style_progress; }
    lv_style_t* getChartStyle() { return &style_chart; }
    
    // Color helpers
    static lv_color_t getColorForValue(float value, int warning_threshold, int danger_threshold);
    
private:
    lv_style_t style_card;
    lv_style_t style_progress;
    lv_style_t style_chart;
    
    bool styles_initialized = false;
};

#endif // LVGL_STYLES_H

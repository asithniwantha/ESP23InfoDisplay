#pragma once
#include "lvgl.h"

class UIStyleManager {
public:
    UIStyleManager();
    void initStyles();

    lv_style_t style_card;
    lv_style_t style_progress;
    lv_style_t style_chart;
};
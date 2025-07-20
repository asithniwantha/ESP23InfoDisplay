#pragma once
#include "lvgl.h"
#include "ui/UIStyleManager.h"

class UIViewFactory {
public:
    UIViewFactory(UIStyleManager& styles);
    lv_obj_t* createCard(lv_obj_t* parent, lv_coord_t x, lv_coord_t y, lv_coord_t w, lv_coord_t h, const char* title);
    void createTopRowCards(lv_obj_t* parent);
    void createCPUHistoryChart(lv_obj_t* parent);
    void createNetworkChart(lv_obj_t* parent);
    void createVolumeControl(lv_obj_t* parent);
    void createTemperatureGauge(lv_obj_t* parent);

private:
    UIStyleManager& styleManager;
};
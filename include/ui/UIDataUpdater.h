#pragma once
#include "lvgl.h"
#include "SystemData.h"

class UIDataUpdater {
public:
    UIDataUpdater();
    void updateUI(SystemData& data);
    void updateProgressBar(lv_obj_t* bar, int32_t value, lv_color_t color);
    void updateCPUHistoryChart(float* history, int historySize);
    void updateNetworkChart(float speed);
    void updateVolumeSlider(int volume);
    void updateTemperatureGauge(int temp);
    lv_color_t getColorForValue(float value, int warning_threshold, int danger_threshold);
};
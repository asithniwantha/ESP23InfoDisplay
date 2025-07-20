#ifndef LVGL_CHARTS_H
#define LVGL_CHARTS_H

#include <lvgl.h>
#include "SystemData.h"
#include "LVGLStyles.h"
#include "UIConfig.h"

class LVGLCharts {
public:
    LVGLCharts(LVGLStyles* styles);
    ~LVGLCharts();
    
    void createCPUHistoryChart(lv_obj_t* parent);
    void createNetworkChart(lv_obj_t* parent);
    void createVolumeControl(lv_obj_t* parent);
    
    void updateCharts(SystemData& data, bool forceUpdate);
    void updateCPUHistoryChart(float* history, int historySize);
    void updateNetworkChart(float speed);
    void updateVolumeSlider(int volume);
    
    // Volume touch handling
    void handleVolumeTouch(int x, int y, SystemData& data);
    
private:
    LVGLStyles* styles;
    
    // CPU History Chart
    lv_obj_t* cpu_history_chart;
    lv_chart_series_t* cpu_history_series;
    
    // Network Chart
    lv_obj_t* network_chart;
    lv_chart_series_t* network_series;
    lv_obj_t* network_label;
    
    // Volume Control
    lv_obj_t* volume_card;
    lv_obj_t* volume_slider;
    lv_obj_t* volume_label;
};

#endif // LVGL_CHARTS_H

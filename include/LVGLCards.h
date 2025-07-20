#ifndef LVGL_CARDS_H
#define LVGL_CARDS_H

#include <lvgl.h>
#include "SystemData.h"
#include "LVGLStyles.h"
#include "UIConfig.h"

class LVGLCards {
public:
    LVGLCards(LVGLStyles* styles);
    ~LVGLCards();
    
    void createTopRowCards(lv_obj_t* parent);
    void updateCards(SystemData& data, bool forceUpdate);
    
    // Individual card creation
    void createCPUCard(lv_obj_t* parent, int x_position);
    void createRAMCard(lv_obj_t* parent, int x_position);
    void createTempCard(lv_obj_t* parent, int x_position);
    void createDiskCard(lv_obj_t* parent, int x_position);
    
    // Card update methods
    void updateCPUCard(float cpuUsage, bool forceUpdate);
    void updateRAMCard(float ramUsage, bool forceUpdate);
    void updateDiskCard(float diskUsage, bool forceUpdate);
    void updateTemperatureCard(int temperature, bool forceUpdate);
    
    // Utility methods
    lv_obj_t* createCard(lv_obj_t* parent, lv_coord_t x, lv_coord_t y, lv_coord_t w, lv_coord_t h, const char* title);
    void updateProgressBar(lv_obj_t* bar, int32_t value, lv_color_t color);
    
private:
    LVGLStyles* styles;
    SystemStats lastStats;
    
    // Card objects
    lv_obj_t* cpu_card;
    lv_obj_t* ram_card;
    lv_obj_t* temp_card;
    lv_obj_t* disk_card;
    
    // Progress bars and labels
    lv_obj_t* cpu_bar;
    lv_obj_t* ram_bar;
    lv_obj_t* disk_bar;
    lv_obj_t* cpu_label;
    lv_obj_t* ram_label;
    lv_obj_t* disk_label;
    
    // Temperature gauge components
    lv_obj_t* temp_gauge;
    lv_meter_indicator_t* temp_needle;
    lv_obj_t* temp_label;
    
    void createTemperatureGauge();
    void updateTemperatureGauge(int temp);
};

#endif // LVGL_CARDS_H

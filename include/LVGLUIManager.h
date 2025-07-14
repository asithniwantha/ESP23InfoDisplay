#ifndef LVGL_UI_MANAGER_H
#define LVGL_UI_MANAGER_H

#include <lvgl.h>
#include "SystemData.h"
#include "UIConfig.h"

class LVGLUIManager {
public:
    LVGLUIManager();
    void init();
    void updateUI(SystemData& data);
    void showStartupScreen();
    void showWiFiInfo(String ip, String mdns, int port);
    void showUDPText(String text);
    void showTouchDebug(int x, int y, int z);
    void handleTouch(int x, int y);
    void showFullScreenChart(int chartType);
    void returnToMainUI();
    
private:
    // Main screen objects
    lv_obj_t* main_screen;
    lv_obj_t* startup_screen;
    lv_obj_t* status_bar;
    
    // Card objects
    lv_obj_t* cpu_card;
    lv_obj_t* ram_card;
    lv_obj_t* disk_card;
    lv_obj_t* temp_card;
    
    // Progress bars
    lv_obj_t* cpu_bar;
    lv_obj_t* ram_bar;
    lv_obj_t* disk_bar;
    lv_obj_t* temp_bar;
    
    // Charts for graphs
    lv_obj_t* cpu_chart;
    lv_obj_t* ram_chart;
    lv_obj_t* temp_chart;
    lv_chart_series_t* cpu_series;
    lv_chart_series_t* ram_series;
    lv_chart_series_t* temp_series;
    
    // Info cards
    lv_obj_t* network_card;
    lv_obj_t* volume_card;
    lv_obj_t* time_label;
    
    // Labels for values
    lv_obj_t* cpu_label;
    lv_obj_t* ram_label;
    lv_obj_t* disk_label;
    lv_obj_t* temp_label;
    lv_obj_t* network_label;
    lv_obj_t* volume_label;
    
    // Helper methods
    void createMainScreen();
    void createStatusBar();
    void createSystemCards();
    void createProgressBars();
    void createInfoCards();
    void createCharts();
    lv_obj_t* createCard(lv_obj_t* parent, lv_coord_t x, lv_coord_t y, lv_coord_t w, lv_coord_t h, const char* title);
    lv_obj_t* createStandardChart(lv_coord_t y_pos, lv_color_t color);
    lv_obj_t* createChartIcon(lv_coord_t x, lv_coord_t y, const char* symbol, lv_color_t color);
    void updateCard(lv_obj_t* card, lv_obj_t* label, const char* value, lv_color_t color);
    void updateProgressBar(lv_obj_t* bar, int32_t value, lv_color_t color);
    void updateChart(lv_chart_series_t* series, int32_t value);
    void updateAllCharts(int32_t cpu_val, int32_t ram_val, int32_t temp_val);
    lv_color_t getColorForValue(float value, int warning_threshold, int danger_threshold);
    
    // Style objects
    lv_style_t style_card;
    lv_style_t style_card_dark;
    lv_style_t style_progress;
    lv_style_t style_chart;
    
    bool ui_initialized = false;
    SystemStats lastStats;
    unsigned long lastForceUpdate = 0;
    
    // Full screen chart state
    bool isFullScreenMode = false;
    int currentFullScreenChart = -1; // -1: none, others use ChartType enum
    lv_obj_t* fullscreen_chart;
    lv_chart_series_t* fullscreen_series;
};

#endif // LVGL_UI_MANAGER_H

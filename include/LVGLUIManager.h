#ifndef LVGL_UI_MANAGER_H
#define LVGL_UI_MANAGER_H

#include <lvgl.h>
#include "SystemData.h"
#include "UIConfig.h"

// Forward declaration
class NetworkManager;

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
    void handleVolumeTouch(int x, int y, SystemData& data);
    void showClockScreen();
    void updateClockScreen();
    void updateClockScreen(NetworkManager* networkManager);
    bool hasRecentData();
    void setDataReceived();
    void checkDataTimeout(); // Check if we should switch to clock due to timeout

    // Loading screen for WiFi
    void showWiFiLoadingScreen();
    void hideWiFiLoadingScreen();
    
private:
    // Main screen objects
    lv_obj_t* main_screen;
    lv_obj_t* startup_screen;
    lv_obj_t* clock_screen;
    
    // Landscape layout cards (top row)
    lv_obj_t* cpu_card;
    lv_obj_t* ram_card;
    lv_obj_t* temp_card;
    lv_obj_t* disk_card;
    
    // Progress bars and gauges
    lv_obj_t* cpu_bar;
    lv_obj_t* ram_bar;
    lv_obj_t* disk_bar;
    lv_obj_t* temp_gauge;
    lv_meter_indicator_t* temp_needle;
    
    // Charts
    lv_obj_t* cpu_history_chart;
    lv_obj_t* network_chart;
    lv_chart_series_t* cpu_history_series;
    lv_chart_series_t* network_series;
    
    // Volume control
    lv_obj_t* volume_card;
    lv_obj_t* volume_slider;
    
    // Labels for values
    lv_obj_t* cpu_label;
    lv_obj_t* ram_label;
    lv_obj_t* disk_label;
    lv_obj_t* temp_label;
    lv_obj_t* network_label;
    lv_obj_t* volume_label;
    
    // Clock screen objects
    lv_obj_t* clock_time_label;
    lv_obj_t* clock_date_label;
    lv_obj_t* clock_status_label;
    lv_obj_t* clock_ampm_label;  // AM/PM indicator
    
    // Helper methods
    void createMainScreen();
    void createLandscapeUI();
    void createTopRowCards();
    void createCPUHistoryChart();
    void createNetworkChart();
    void createVolumeControl();
    void createTemperatureGauge();
    lv_obj_t* createCard(lv_obj_t* parent, lv_coord_t x, lv_coord_t y, lv_coord_t w, lv_coord_t h, const char* title);
    void updateCard(lv_obj_t* card, lv_obj_t* label, const char* value, lv_color_t color);
    void updateProgressBar(lv_obj_t* bar, int32_t value, lv_color_t color);
    void updateCPUHistoryChart(float* history, int historySize);
    void updateNetworkChart(float speed);
    void updateVolumeSlider(int volume);
    void updateTemperatureGauge(int temp);
    lv_color_t getColorForValue(float value, int warning_threshold, int danger_threshold);
    
    // Style objects
    lv_style_t style_card;
    lv_style_t style_card_dark;
    lv_style_t style_progress;
    lv_style_t style_chart;
    
    bool ui_initialized = false;
    SystemStats lastStats;
    unsigned long lastForceUpdate = 0;
    unsigned long lastDataReceived = 0;
    bool isClockMode = false;
    
    // Full screen chart state
    bool isFullScreenMode = false;
    int currentFullScreenChart = -1; // -1: none, others use ChartType enum
    lv_obj_t* fullscreen_chart;
    lv_chart_series_t* fullscreen_series;
};

#endif // LVGL_UI_MANAGER_H

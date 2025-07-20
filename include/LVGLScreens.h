#ifndef LVGL_SCREENS_H
#define LVGL_SCREENS_H

#include <lvgl.h>
#include "UIConfig.h"
#include "lv_font_digital_80.h"

// Forward declaration
class NetworkManager;

class LVGLScreens {
public:
    LVGLScreens();
    ~LVGLScreens();
    
    // Screen management
    void showStartupScreen();
    void showWiFiInfo(String ip, String mdns, int port);
    void showUDPText(String text);
    void showWiFiLoadingScreen();
    void hideWiFiLoadingScreen();
    
    // Clock screen
    void showClockScreen();
    void updateClockScreen(NetworkManager* networkManager);
    bool isInClockMode() const { return isClockMode; }
    void setClockMode(bool mode) { isClockMode = mode; }
    
    // Data timeout management
    void setDataReceived();
    bool hasRecentData();
    void checkDataTimeout();
    
private:
    // Screen objects
    lv_obj_t* startup_screen;
    lv_obj_t* clock_screen;
    lv_obj_t* wifi_loading_screen;
    
    // Clock screen components
    lv_obj_t* clock_time_label;
    lv_obj_t* clock_date_label;
    lv_obj_t* clock_status_label;
    lv_obj_t* clock_ampm_label;
    lv_obj_t* clock_seconds_label;
    
    // State management
    bool isClockMode = false;
    unsigned long lastDataReceived = 0;
};

#endif // LVGL_SCREENS_H

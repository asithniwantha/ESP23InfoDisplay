#ifndef LVGL_UI_MANAGER_H
#define LVGL_UI_MANAGER_H

#include <lvgl.h>
#include "SystemData.h"
#include "UIConfig.h"
#include "LVGLStyles.h"
#include "LVGLCards.h"
#include "LVGLCharts.h"
#include "LVGLScreens.h"
#include "LVGLTouch.h"

// Forward declaration
class NetworkManager;

class LVGLUIManager {
public:
    LVGLUIManager();
    ~LVGLUIManager();
    
    void init();
    void updateUI(SystemData& data);
    
    // Delegate methods to components
    void showStartupScreen();
    void showWiFiInfo(String ip, String mdns, int port);
    void showUDPText(String text);
    void showTouchDebug(int x, int y, int z);
    void handleTouch(int x, int y);
    void showFullScreenChart(int chartType);
    void returnToMainUI();
    void handleVolumeTouch(int x, int y, SystemData& data);
    void showClockScreen();
    void updateClockScreen(NetworkManager* networkManager);
    bool hasRecentData();
    void setDataReceived();
    void checkDataTimeout();
    void showWiFiLoadingScreen();
    void hideWiFiLoadingScreen();
    
private:
    // Component instances
    LVGLStyles* styles;
    LVGLCards* cards;
    LVGLCharts* charts;
    LVGLScreens* screens;
    LVGLTouch* touch;
    
    // Main screen object
    lv_obj_t* main_screen;
    
    // Helper methods
    void createMainScreen();
    void createLandscapeUI();
    
    // State management
    bool ui_initialized = false;
    unsigned long lastForceUpdate = 0;
};

#endif // LVGL_UI_MANAGER_H

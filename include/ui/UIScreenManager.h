#pragma once
#include "lvgl.h"
#include "NetworkManager.h"

class UIScreenManager {
public:
    UIScreenManager();
    void showStartupScreen();
    void showWiFiInfo(String ip, String mdns, int port);
    void showUDPText(String text);
    void showWiFiLoadingScreen();
    void hideWiFiLoadingScreen();
    void showClockScreen();
    void updateClockScreen(NetworkManager* networkManager);
    void returnToMainUI();
    lv_obj_t* getMainScreen();

private:
    lv_obj_t* main_screen;
    lv_obj_t* startup_screen;
    lv_obj_t* clock_screen;
    lv_obj_t* wifi_loading_screen;
};
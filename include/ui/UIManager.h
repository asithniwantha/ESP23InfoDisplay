#pragma once
#include "lvgl.h"
#include "SystemData.h"
#include "NetworkManager.h"
#include "LVGLDriver.h"
#include "ui/UIStyleManager.h"
#include "ui/UIViewFactory.h"
#include "ui/UIDataUpdater.h"
#include "ui/UIScreenManager.h"
#include "ui/UITouchHandler.h"
#include "TouchManager.h"

class UIManager {
public:
    UIManager(SystemData& systemData, NetworkManager& networkManager);
    void init();
    void tick();
    void update();
    void showStartupScreen();
    void showWiFiInfo(String ip, String mdns, int port);
    void showUDPText(String text);
    void showWiFiLoadingScreen();
    void hideWiFiLoadingScreen();
    void showClockScreen();
    void setDataReceived();

private:
    UIScreenManager screenManager;
    UITouchHandler touchHandler;
    SystemData& data;
    NetworkManager& netManager;
    LVGLDriver lvglDriver;
    TouchManager touchManager;
    UIStyleManager styleManager;
    UIViewFactory viewFactory;
    UIDataUpdater dataUpdater;
    // UI state
    bool ui_initialized;
    unsigned long lastDataReceived;
    bool isClockMode;
    unsigned long lastForceUpdate;
    SystemStats lastStats;

    void handleTouch();
    void checkDataTimeout();
    bool hasRecentData();

private:
    UIScreenManager screenManager;
    UITouchHandler touchHandler;
    SystemData& data;
    NetworkManager& netManager;
    LVGLDriver lvglDriver;
    TouchManager touchManager;
    UIStyleManager styleManager;
    UIViewFactory viewFactory;
    UIDataUpdater dataUpdater;
    // UI state
    bool ui_initialized;
    unsigned long lastDataReceived;
    bool isClockMode;
    unsigned long lastForceUpdate;
    SystemStats lastStats;

    void handleTouch();
    void checkDataTimeout();
    bool hasRecentData();
};

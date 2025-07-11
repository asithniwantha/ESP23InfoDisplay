#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <TFT_eSPI.h>
#include <ArduinoJson.h>
#include "SystemData.h"
#include "Defines.h"
#include "UIRenderer.h"
#include "GraphRenderer.h"
#include "ScreenManager.h"

class DisplayManager {
public:
    DisplayManager();
    void begin();
    void showStartupScreen();
    void displayWiFiInfo(String ip, String mdns, int port);
    void displayUDPText(String text);
    void displayModernUI(SystemData& data);
    void printTouchToDisplay(int x, int y, int z);
    void resetUIState();

private:
    TFT_eSPI tft;
    UIRenderer uiRenderer;
    GraphRenderer graphRenderer;
    ScreenManager screenManager;
    bool uiInitialized = false;
    SystemStats lastDrawnStats;
    
    void initializeUI();
    void drawInitialUIElements();
    void updateCards(SystemStats& stats);
    void updateProgressBars(SystemStats& stats);
    void updateBottomSection(SystemData& data);
};

#endif // DISPLAY_MANAGER_H


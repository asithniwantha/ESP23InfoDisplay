#ifndef SCREEN_MANAGER_H
#define SCREEN_MANAGER_H

#include <TFT_eSPI.h>
#include "UIRenderer.h"
#include "Defines.h"

class ScreenManager {
public:
    ScreenManager(TFT_eSPI& display, UIRenderer& uiRenderer);
    
    void showStartupScreen();
    void displayWiFiInfo(String ip, String mdns, int port);
    void displayUDPText(String text);
    void printTouchToDisplay(int x, int y, int z);

private:
    TFT_eSPI& tft;
    UIRenderer& ui;
};

#endif // SCREEN_MANAGER_H

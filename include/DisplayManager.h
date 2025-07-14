#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <lvgl.h>
#include <ArduinoJson.h>
#include "SystemData.h"
#include "Defines.h"
#include "UIConfig.h"
#include "LVGLDriver.h"
#include "LVGLUIManager.h"

// Forward declaration
class NetworkManager;

class DisplayManager {
public:
    DisplayManager();
    void begin();
    void tick();
    void showStartupScreen();
    void displayWiFiInfo(String ip, String mdns, int port);
    void displayUDPText(String text);
    void displayModernUI(SystemData& data);
    void printTouchToDisplay(int x, int y, int z);
    void handleElementTouch(int x, int y);
    void resetUIState();
    void updateClock(); // For updating clock when no data
    void updateClock(NetworkManager* networkManager); // Overloaded with NetworkManager
    void showClockAfterSetup(); // Show clock after setup is complete
    void checkDataTimeout(); // Check if we should switch to clock due to timeout
    
    // Volume control methods
    void handleVolumeTouch(int x, int y, SystemData& data);
    void onVolumeChanged(int newVolume);

private:
    LVGLDriver lvglDriver;
    LVGLUIManager uiManager;
    bool uiInitialized = false;
};

#endif // DISPLAY_MANAGER_H


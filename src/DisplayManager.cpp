#include "DisplayManager.h"

DisplayManager::DisplayManager() {
    uiInitialized = false;
}

void DisplayManager::begin() {
    lvglDriver.init();
    uiManager.init();
    uiInitialized = true;
}

void DisplayManager::tick() {
    lvglDriver.tick();
}

void DisplayManager::showStartupScreen() {
    uiManager.showStartupScreen();
}

void DisplayManager::displayWiFiInfo(String ip, String mdns, int port) {
    uiManager.showWiFiInfo(ip, mdns, port);
    uiInitialized = false; // Reset state for future UI changes
}

void DisplayManager::displayUDPText(String text) {
    uiManager.showUDPText(text);
    uiInitialized = false; // Reset state for future UI changes
}

void DisplayManager::printTouchToDisplay(int x, int y, int z) {
    uiManager.showTouchDebug(x, y, z);
    uiInitialized = false; // Reset state for future UI changes
}

void DisplayManager::handleElementTouch(int x, int y) {
    uiManager.handleTouch(x, y);
}

void DisplayManager::resetUIState() {
    uiInitialized = false;
}

void DisplayManager::displayModernUI(SystemData& data) {
    if (!uiInitialized) {
        // Reinitialize the main UI
        uiManager.init();
        uiInitialized = true;
    }
    
    uiManager.updateUI(data);
}

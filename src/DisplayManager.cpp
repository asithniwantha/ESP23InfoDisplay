#include "DisplayManager.h"
#include "UIConfig.h"

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

void DisplayManager::showWiFiLoadingScreen() {
    uiManager.showWiFiLoadingScreen();
}

void DisplayManager::hideWiFiLoadingScreen() {
    uiManager.hideWiFiLoadingScreen();
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
    
    // Mark that we've received data
    uiManager.setDataReceived();
    
    uiManager.updateUI(data);
}

void DisplayManager::handleVolumeTouch(int x, int y, SystemData& data) {
    // Calculate volume based on touch position within volume control area
    int sliderX = UILayout::VOLUME_CONTROL_X + 25; // Account for speaker icon
    int sliderW = UILayout::BOTTOM_CARD_WIDTH - 35; // Slider width
    
    if (x >= sliderX && x <= sliderX + sliderW) {
        float newVolume = ((x - sliderX) * 100.0f) / sliderW;
        newVolume = constrain(newVolume, 0, 100);
        
        data.setVolume((int)newVolume);
        onVolumeChanged((int)newVolume);
        
        Serial.printf("Volume changed to: %d%%\n", (int)newVolume);
    }
}

void DisplayManager::onVolumeChanged(int newVolume) {
    // This could send a UDP command to change system volume
    // For now, just log the change
    Serial.printf("Volume control: Setting volume to %d%%\n", newVolume);
}

void DisplayManager::updateClock() {
    uiManager.updateClockScreen();
}

void DisplayManager::updateClock(NetworkManager* networkManager) {
    uiManager.updateClockScreen(networkManager);
}

void DisplayManager::showClockAfterSetup() {
    uiManager.showClockScreen();
}

void DisplayManager::checkDataTimeout() {
    uiManager.checkDataTimeout();
}

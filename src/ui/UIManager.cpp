#include "ui/UIManager.h"
#include "UIConfig.h"

UIManager::UIManager(SystemData& systemData, NetworkManager& networkManager)
    : data(systemData),
      netManager(networkManager),
      lvglDriver(),
      touchManager(XPT2046_CS, XPT2046_IRQ),
      styleManager(),
      viewFactory(styleManager),
      dataUpdater(),
      screenManager(),
      touchHandler(),
      ui_initialized(false),
      lastDataReceived(0),
      isClockMode(false),
      lastForceUpdate(0) {}

void UIManager::init() {
    lvglDriver.init();
    styleManager.initStyles();
    screenManager.showStartupScreen();
    Serial.println("[DEBUG] Entered UIManager::showStartupScreen()");
    // ...existing code...
    Serial.println("[DEBUG] Leaving UIManager::showStartupScreen()");
    touchManager.begin();
    ui_initialized = true;
}

void UIManager::tick() {
    lvglDriver.tick();
    handleTouch();
    checkDataTimeout();
    screenManager.updateClockScreen(&netManager);
}

void UIManager::update() {
    if (!ui_initialized) return;

    if (isClockMode) {
        screenManager.updateClockScreen(&netManager);
        return;
    }

    lv_scr_load(screenManager.getMainScreen());

    SystemStats& stats = data.getCurrentData();
    bool forceUpdate = (millis() - lastForceUpdate) >= SystemThresholds::FORCE_UPDATE_INTERVAL;
    if (forceUpdate) {
        lastForceUpdate = millis();
    }

    dataUpdater.updateUI(data);
    lastStats = stats;
}

void UIManager::showStartupScreen() {
    screenManager.showStartupScreen();
}

void UIManager::showWiFiInfo(String ip, String mdns, int port) {    
    Serial.println("[DEBUG] Entered UIManager::showWiFiInfo()");
screenManager.showWiFiInfo(ip, mdns, port);
    Serial.println("[DEBUG] Leaving UIManager::showWiFiInfo()");
}

void UIManager::showUDPText(String text) {
    screenManager.showUDPText(text);
}

void UIManager::showWiFiLoadingScreen() {
    screenManager.showWiFiLoadingScreen();
}

void UIManager::hideWiFiLoadingScreen() {
    screenManager.hideWiFiLoadingScreen();
}

void UIManager::showClockScreen() {
    isClockMode = true;
    screenManager.showClockScreen();
}

void UIManager::handleTouch() {
    if (touchManager.isTouched()) {
        TS_Point p = touchManager.getPoint();
        touchManager.calibrate(p);
        touchHandler.handleTouch(p.x, p.y);
        touchHandler.handleVolumeTouch(p.x, p.y, data);
    }
}

void UIManager::checkDataTimeout() {
    if (!isClockMode && lastDataReceived > 0 && !hasRecentData()) {
        showClockScreen();
    }
}

bool UIManager::hasRecentData() {
    if (lastDataReceived == 0) return false;
    return (millis() - lastDataReceived) < 10000;
}

void UIManager::setDataReceived() {
    lastDataReceived = millis();
    if (isClockMode) {
        isClockMode = false;
        lv_scr_load(screenManager.getMainScreen());
    }
}

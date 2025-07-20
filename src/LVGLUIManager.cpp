#include "LVGLUIManager.h"
#include "NetworkManager.h"
#include <Arduino.h>

LVGLUIManager::LVGLUIManager() {
    main_screen = nullptr;
    ui_initialized = false;
    lastForceUpdate = 0;
    
    // Create component instances
    styles = new LVGLStyles();
    cards = new LVGLCards(styles);
    charts = new LVGLCharts(styles);
    screens = new LVGLScreens();
    touch = new LVGLTouch();
}

LVGLUIManager::~LVGLUIManager() {
    delete styles;
    delete cards;
    delete charts;
    delete screens;
    delete touch;
}

void LVGLUIManager::init() {
    // Initialize styles first
    styles->init();
    
    // Create main screen
    createMainScreen();
}

void LVGLUIManager::createMainScreen() {
    main_screen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(main_screen, lv_color_hex(UIColors::DARK), 0);
    
    // Create landscape layout
    createLandscapeUI();
    
    ui_initialized = true;
}

void LVGLUIManager::createLandscapeUI() {
    cards->createTopRowCards(main_screen);
    charts->createCPUHistoryChart(main_screen);
    charts->createNetworkChart(main_screen);
    charts->createVolumeControl(main_screen);
}

void LVGLUIManager::updateUI(SystemData& data) {
    if (!ui_initialized) return;
    
    // If we're in clock mode, don't update the main UI
    if (screens->isInClockMode()) {
        screens->updateClockScreen(nullptr); // Keep updating the clock
        return;
    }
    
    // Make sure we're on the main screen
    lv_scr_load(main_screen);
    
    // Check if we need a force update
    unsigned long currentTime = millis();
    bool forceUpdate = (currentTime - lastForceUpdate) >= SystemThresholds::FORCE_UPDATE_INTERVAL;
    if (forceUpdate) {
        lastForceUpdate = currentTime;
    }
    
    // Update all components
    cards->updateCards(data, forceUpdate);
    charts->updateCharts(data, forceUpdate);
}

// Delegate methods to appropriate components
void LVGLUIManager::showStartupScreen() {
    screens->showStartupScreen();
}

void LVGLUIManager::showWiFiInfo(String ip, String mdns, int port) {
    screens->showWiFiInfo(ip, mdns, port);
}

void LVGLUIManager::showUDPText(String text) {
    screens->showUDPText(text);
}

void LVGLUIManager::showTouchDebug(int x, int y, int z) {
    touch->showTouchDebug(x, y, z);
}

void LVGLUIManager::handleTouch(int x, int y) {
    touch->handleTouch(x, y);
}

void LVGLUIManager::showFullScreenChart(int chartType) {
    touch->showFullScreenChart(chartType);
}

void LVGLUIManager::returnToMainUI() {
    if (!touch->isInFullScreenMode()) return;
    
    touch->returnToMainUI();
    
    // Recreate the main UI with landscape layout
    lv_obj_clean(main_screen);
    createLandscapeUI();
    
    lv_scr_load(main_screen);
}

void LVGLUIManager::handleVolumeTouch(int x, int y, SystemData& data) {
    charts->handleVolumeTouch(x, y, data);
}

void LVGLUIManager::showClockScreen() {
    screens->showClockScreen();
}

void LVGLUIManager::updateClockScreen(NetworkManager* networkManager) {
    screens->updateClockScreen(networkManager);
}

bool LVGLUIManager::hasRecentData() {
    return screens->hasRecentData();
}

void LVGLUIManager::setDataReceived() {
    screens->setDataReceived();
    
    // If we were in clock mode, return to main UI
    if (screens->isInClockMode()) {
        screens->setClockMode(false);
        lv_scr_load(main_screen);
        Serial.println("Returning to main UI - UDP data received");
    }
}

void LVGLUIManager::checkDataTimeout() {
    screens->checkDataTimeout();
}

void LVGLUIManager::showWiFiLoadingScreen() {
    screens->showWiFiLoadingScreen();
}

void LVGLUIManager::hideWiFiLoadingScreen() {
    screens->hideWiFiLoadingScreen();
}



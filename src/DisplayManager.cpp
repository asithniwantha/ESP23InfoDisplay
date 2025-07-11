#include "DisplayManager.h"

DisplayManager::DisplayManager() : tft(TFT_eSPI()), 
                                   uiRenderer(tft), 
                                   graphRenderer(tft),
                                   screenManager(tft, uiRenderer) {}

void DisplayManager::begin() {
    tft.init();
    tft.setRotation(1);
}

void DisplayManager::showStartupScreen() {
    screenManager.showStartupScreen();
}

void DisplayManager::displayWiFiInfo(String ip, String mdns, int port) {
    screenManager.displayWiFiInfo(ip, mdns, port);
}

void DisplayManager::displayUDPText(String text) {
    // Reset UI state when switching to text mode
    uiInitialized = false;
    screenManager.displayUDPText(text);
}

void DisplayManager::printTouchToDisplay(int x, int y, int z) {
    // Reset UI state when switching to touch mode
    uiInitialized = false;
    screenManager.printTouchToDisplay(x, y, z);
}

void DisplayManager::resetUIState() {
    uiInitialized = false;
}

void DisplayManager::displayModernUI(SystemData& data) {
    SystemStats& stats = data.getCurrentData();
    
    // Initialize UI only once or after mode changes
    if (!uiInitialized || !stats.initialized) {
        initializeUI();
        uiInitialized = true;
        stats.initialized = true;
        lastDrawnStats = stats; // Store initial state
    }
    
    // Update only sections that have changed
    updateCards(stats);
    updateProgressBars(stats);
    updateBottomSection(data);
    
    // Update the last drawn state
    lastDrawnStats = stats;
}

void DisplayManager::initializeUI() {
    // Clear screen with modern background - only do this once
    tft.fillScreen(COLOR_BG);
    
    // Draw status bar
    uiRenderer.drawStatusBar();
    
    // Show all UI elements with default values immediately
    drawInitialUIElements();
}

void DisplayManager::drawInitialUIElements() {
    // Define consistent layout constants
    const int LEFT_MARGIN = 5;
    const int CARD_SPACING = 5;
    const int CARD_HEIGHT = 35;
    const int CARD_WIDTH = 75;
    const int CARDS_Y = 25;
    const int PROGRESS_BAR_Y = 65;
    const int PROGRESS_BAR_HEIGHT = 12;
    const int INFO_CARDS_Y = 90;
    const int INFO_CARD_HEIGHT = 20;
    const int GRAPH_START_Y = 136;
    const int GRAPH_WIDTH = 310;
    const int GRAPH_HEIGHT = 25;
    const int GRAPH_SPACING = 32;
    
    // Row 1: Draw all main cards with default values
    int cardX = LEFT_MARGIN;
    uiRenderer.drawCard(cardX, CARDS_Y, CARD_WIDTH, CARD_HEIGHT, "CPU", "0.0%", COLOR_SUCCESS);
    cardX += CARD_WIDTH + CARD_SPACING;
    
    uiRenderer.drawCard(cardX, CARDS_Y, CARD_WIDTH, CARD_HEIGHT, "RAM", "0.0%", COLOR_SUCCESS);
    cardX += CARD_WIDTH + CARD_SPACING;
    
    uiRenderer.drawCard(cardX, CARDS_Y, CARD_WIDTH, CARD_HEIGHT, "DISK", "0.0%", COLOR_SUCCESS);
    cardX += CARD_WIDTH + CARD_SPACING;
    
    uiRenderer.drawCard(cardX, CARDS_Y, CARD_WIDTH, CARD_HEIGHT, "TEMP", "0°C", COLOR_INFO);
    
    // Row 2: Draw all progress bars with 0% values
    int progressX = LEFT_MARGIN;
    uiRenderer.drawProgressBar(progressX, PROGRESS_BAR_Y, CARD_WIDTH, PROGRESS_BAR_HEIGHT, 0, COLOR_SUCCESS);
    progressX += CARD_WIDTH + CARD_SPACING;
    
    uiRenderer.drawProgressBar(progressX, PROGRESS_BAR_Y, CARD_WIDTH, PROGRESS_BAR_HEIGHT, 0, COLOR_SUCCESS);
    progressX += CARD_WIDTH + CARD_SPACING;
    
    uiRenderer.drawProgressBar(progressX, PROGRESS_BAR_Y, CARD_WIDTH, PROGRESS_BAR_HEIGHT, 0, COLOR_SUCCESS);
    progressX += CARD_WIDTH + CARD_SPACING;
    
    uiRenderer.drawProgressBar(progressX, PROGRESS_BAR_Y, CARD_WIDTH, PROGRESS_BAR_HEIGHT, 0, COLOR_INFO);
    
    // Row 3: Draw info cards with default values
    cardX = LEFT_MARGIN;
    uiRenderer.drawCard(cardX, INFO_CARDS_Y, CARD_WIDTH, INFO_CARD_HEIGHT, "Net", "0.0MB/s", COLOR_INFO);
    cardX += CARD_WIDTH + CARD_SPACING;
    
    uiRenderer.drawCard(cardX, INFO_CARDS_Y, CARD_WIDTH, INFO_CARD_HEIGHT, "Vol", "0%", COLOR_PRIMARY);
    
    // Display waiting message
    tft.setTextColor(COLOR_MUTED, COLOR_BG);
    tft.drawString("Waiting for data...", LEFT_MARGIN + 200, INFO_CARDS_Y + 5, 1);
    
    // Row 4-6: Draw empty graph frames
    tft.drawRect(LEFT_MARGIN, GRAPH_START_Y, GRAPH_WIDTH - 35, GRAPH_HEIGHT, COLOR_MUTED);
    tft.drawRect(LEFT_MARGIN, GRAPH_START_Y + GRAPH_SPACING, GRAPH_WIDTH - 35, GRAPH_HEIGHT, COLOR_MUTED);
    tft.drawRect(LEFT_MARGIN, GRAPH_START_Y + (GRAPH_SPACING * 2), GRAPH_WIDTH - 35, GRAPH_HEIGHT, COLOR_MUTED);
    
    // Add current value placeholders on the right of graphs
    tft.setTextColor(COLOR_SUCCESS, COLOR_BG);
    tft.drawString("0%", LEFT_MARGIN + GRAPH_WIDTH - 32, GRAPH_START_Y + (GRAPH_HEIGHT - 8) / 2, 1);
    
    tft.setTextColor(COLOR_WARNING, COLOR_BG);
    tft.drawString("0%", LEFT_MARGIN + GRAPH_WIDTH - 32, GRAPH_START_Y + GRAPH_SPACING + (GRAPH_HEIGHT - 8) / 2, 1);
    
    tft.setTextColor(COLOR_DANGER, COLOR_BG);
    tft.drawString("0°", LEFT_MARGIN + GRAPH_WIDTH - 32, GRAPH_START_Y + (GRAPH_SPACING * 2) + (GRAPH_HEIGHT - 8) / 2, 1);
}

void DisplayManager::updateCards(SystemStats& stats) {
    // Define consistent layout constants
    const int LEFT_MARGIN = 5;
    const int CARD_SPACING = 5;
    const int CARD_HEIGHT = 35;
    const int CARD_WIDTH = 75;
    const int CARDS_Y = 25;
    const int INFO_CARDS_Y = 65;
    
    // Row 1: Main system cards - perfectly aligned
    int cardX = LEFT_MARGIN;
    
    // Only update CPU card if value changed significantly
    if (abs(stats.cpuUsage - lastDrawnStats.cpuUsage) > 0.5) {
        uiRenderer.drawCard(cardX, CARDS_Y, CARD_WIDTH, CARD_HEIGHT, "CPU", String(stats.cpuUsage, 1) + "%", 
                 stats.cpuUsage > 80 ? COLOR_DANGER : stats.cpuUsage > 60 ? COLOR_WARNING : COLOR_SUCCESS);
    }
    cardX += CARD_WIDTH + CARD_SPACING;
    
    // Only update RAM card if value changed significantly
    if (abs(stats.ramUsage - lastDrawnStats.ramUsage) > 0.5) {
        uiRenderer.drawCard(cardX, CARDS_Y, CARD_WIDTH, CARD_HEIGHT, "RAM", String(stats.ramUsage, 1) + "%", 
                 stats.ramUsage > 80 ? COLOR_DANGER : stats.ramUsage > 60 ? COLOR_WARNING : COLOR_SUCCESS);
    }
    cardX += CARD_WIDTH + CARD_SPACING;
    
    // Only update DISK card if value changed significantly
    if (abs(stats.diskUsage - lastDrawnStats.diskUsage) > 0.5) {
        uiRenderer.drawCard(cardX, CARDS_Y, CARD_WIDTH, CARD_HEIGHT, "DISK", String(stats.diskUsage, 1) + "%", 
                 stats.diskUsage > 80 ? COLOR_DANGER : stats.diskUsage > 60 ? COLOR_WARNING : COLOR_SUCCESS);
    }
    cardX += CARD_WIDTH + CARD_SPACING;
    
    // Only update TEMP card if value changed
    if (stats.cpuTemp != lastDrawnStats.cpuTemp) {
        uiRenderer.drawCard(cardX, CARDS_Y, CARD_WIDTH, CARD_HEIGHT, "TEMP", String(stats.cpuTemp) + "°C", 
                 stats.cpuTemp > 70 ? COLOR_DANGER : stats.cpuTemp > 50 ? COLOR_WARNING : COLOR_INFO);
    }
    
    // Row 3: Info cards moved to y=90
    const int INFO_CARDS_Y_NEW = 90;
    cardX = LEFT_MARGIN;
    const int INFO_CARD_HEIGHT = 20;
    
    if (abs(stats.networkSpeed - lastDrawnStats.networkSpeed) > 0.1) {
        uiRenderer.drawCard(cardX, INFO_CARDS_Y_NEW, CARD_WIDTH, INFO_CARD_HEIGHT, "Net", String(stats.networkSpeed, 1) + "MB/s", COLOR_INFO);
    }
    cardX += CARD_WIDTH + CARD_SPACING;
    
    if (stats.volumeLevel != lastDrawnStats.volumeLevel) {
        uiRenderer.drawCard(cardX, INFO_CARDS_Y_NEW, CARD_WIDTH, INFO_CARD_HEIGHT, "Vol", String(stats.volumeLevel) + "%", COLOR_PRIMARY);
    }
    
    // Updated timestamp moved to a separate display area or removed for cleaner layout
    if (stats.lastTime != lastDrawnStats.lastTime) {
        // Clear the waiting message area first
        tft.fillRect(LEFT_MARGIN + 200, INFO_CARDS_Y_NEW, 115, 15, COLOR_BG);
        // Display updated time in a small area or status bar instead
        tft.setTextColor(COLOR_MUTED, COLOR_BG);
        tft.drawString("Updated: " + stats.lastTime, LEFT_MARGIN + 200, INFO_CARDS_Y_NEW + 5, 1);
    }
}

void DisplayManager::updateProgressBars(SystemStats& stats) {
    // Define consistent layout constants matching cards exactly
    const int LEFT_MARGIN = 5;
    const int CARD_SPACING = 5;
    const int CARD_WIDTH = 75;
    const int PROGRESS_BAR_Y = 65; // Row 2 position
    const int PROGRESS_BAR_HEIGHT = 12;
    
    // Align progress bars exactly with the cards above them
    int progressX = LEFT_MARGIN;
    
    // CPU progress bar - aligned with CPU card
    if (abs(stats.cpuUsage - lastDrawnStats.cpuUsage) > 0.5) {
        uiRenderer.drawProgressBar(progressX, PROGRESS_BAR_Y, CARD_WIDTH, PROGRESS_BAR_HEIGHT, stats.cpuUsage, 
                       stats.cpuUsage > 80 ? COLOR_DANGER : COLOR_SUCCESS);
    }
    progressX += CARD_WIDTH + CARD_SPACING;
    
    // RAM progress bar - aligned with RAM card
    if (abs(stats.ramUsage - lastDrawnStats.ramUsage) > 0.5) {
        uiRenderer.drawProgressBar(progressX, PROGRESS_BAR_Y, CARD_WIDTH, PROGRESS_BAR_HEIGHT, stats.ramUsage, 
                       stats.ramUsage > 80 ? COLOR_DANGER : COLOR_SUCCESS);
    }
    progressX += CARD_WIDTH + CARD_SPACING;
    
    // DISK progress bar - aligned with DISK card
    if (abs(stats.diskUsage - lastDrawnStats.diskUsage) > 0.5) {
        uiRenderer.drawProgressBar(progressX, PROGRESS_BAR_Y, CARD_WIDTH, PROGRESS_BAR_HEIGHT, stats.diskUsage, 
                       stats.diskUsage > 80 ? COLOR_DANGER : COLOR_SUCCESS);
    }
    progressX += CARD_WIDTH + CARD_SPACING;
    
    // TEMP progress bar - aligned with TEMP card (NEW)
    if (abs(stats.cpuTemp - lastDrawnStats.cpuTemp) > 0.5) {
        // Convert temperature to percentage for progress bar (0-100°C range)
        float tempPercentage = stats.cpuTemp;
        if (tempPercentage > 100.0f) tempPercentage = 100.0f;
        if (tempPercentage < 0.0f) tempPercentage = 0.0f;
        uiRenderer.drawProgressBar(progressX, PROGRESS_BAR_Y, CARD_WIDTH, PROGRESS_BAR_HEIGHT, tempPercentage, 
                       stats.cpuTemp > 70 ? COLOR_DANGER : stats.cpuTemp > 50 ? COLOR_WARNING : COLOR_INFO);
    }
}

void DisplayManager::updateBottomSection(SystemData& data) {
    // Define consistent layout constants
    const int LEFT_MARGIN = 5;
    const int GRAPH_START_Y = 136; // Row 4 position as specified
    const int GRAPH_WIDTH = 310;
    const int GRAPH_HEIGHT = 25;
    const int GRAPH_SPACING = 32; // 168-136=32, 200-168=32
    
    // Update graphs more frequently for smooth scrolling effect
    static int graphUpdateCounter = 0;
    graphUpdateCounter++;
    
    if (graphUpdateCounter >= 1) { // Update graphs every data update for smooth scrolling
        graphUpdateCounter = 0;
        
        int dataPoints = data.getDataPointsToShow();
        
        if (dataPoints > 1) {  // Show graphs if we have at least 2 data points
            // Row 4: CPU Graph at y=136
            graphRenderer.drawScrollingGraph(LEFT_MARGIN, GRAPH_START_Y, GRAPH_WIDTH, GRAPH_HEIGHT, data, 0, COLOR_SUCCESS);
            
            // Row 5: RAM Graph at y=168
            graphRenderer.drawScrollingGraph(LEFT_MARGIN, GRAPH_START_Y + GRAPH_SPACING, GRAPH_WIDTH, GRAPH_HEIGHT, data, 1, COLOR_WARNING);
            
            // Row 6: Temperature Graph at y=200
            graphRenderer.drawScrollingGraph(LEFT_MARGIN, GRAPH_START_Y + (GRAPH_SPACING * 2), GRAPH_WIDTH, GRAPH_HEIGHT, data, 2, COLOR_DANGER);
        }
    }
}

#include "LVGLTouch.h"
#include <Arduino.h>

LVGLTouch::LVGLTouch() {
    isFullScreenMode = false;
    currentFullScreenChart = -1;
}

LVGLTouch::~LVGLTouch() {
    // Nothing to clean up
}

void LVGLTouch::handleTouch(int x, int y) {
    using namespace TouchZones;
    
    // Check touch zones for landscape layout
    if (y >= CARD_Y_MIN && y <= CARD_Y_MAX) {
        // Top row cards
        if (x >= CPU_CARD_X_MIN && x <= CPU_CARD_X_MAX) {
            Serial.println("CPU card touched");
        } else if (x >= RAM_CARD_X_MIN && x <= RAM_CARD_X_MAX) {
            Serial.println("RAM card touched");  
        } else if (x >= TEMP_CARD_X_MIN && x <= TEMP_CARD_X_MAX) {
            Serial.println("Temperature card touched");
        } else if (x >= DISK_CARD_X_MIN && x <= DISK_CARD_X_MAX) {
            Serial.println("Disk card touched");
        }
    } else if (x >= CPU_CHART_X_MIN && x <= CPU_CHART_X_MAX && 
               y >= CPU_CHART_Y_MIN && y <= CPU_CHART_Y_MAX) {
        Serial.println("CPU chart touched");
    } else if (x >= NETWORK_CHART_X_MIN && x <= NETWORK_CHART_X_MAX &&
               y >= NETWORK_CHART_Y_MIN && y <= NETWORK_CHART_Y_MAX) {
        Serial.println("Network chart touched");
    }
    // Volume touch is handled separately in main.cpp or by charts component
}

void LVGLTouch::showTouchDebug(int x, int y, int z) {
    // Simple touch debug display - could be enhanced
    Serial.printf("Touch: X=%d, Y=%d, Z=%d\n", x, y, z);
}

void LVGLTouch::handleVolumeTouch(int x, int y, SystemData& data) {
    // Calculate relative position within volume control and update volume
    int local_x = x - UILayout::VOLUME_CONTROL_X;
    if (local_x >= 25 && local_x <= UILayout::BOTTOM_CARD_WIDTH - 15) {
        int slider_width = UILayout::BOTTOM_CARD_WIDTH - 40;
        float volume_percent = ((local_x - 25) * 100.0f) / slider_width;
        volume_percent = (volume_percent < 0) ? 0 : ((volume_percent > 100) ? 100 : volume_percent);
        
        data.setVolume((int)volume_percent);
        // Note: Volume slider update should be handled by the charts component
    }
}

void LVGLTouch::showFullScreenChart(int chartType) {
    isFullScreenMode = true;
    currentFullScreenChart = chartType;
    Serial.printf("Full screen chart mode: %d\n", chartType);
}

void LVGLTouch::returnToMainUI() {
    if (!isFullScreenMode) return;
    
    isFullScreenMode = false;
    currentFullScreenChart = -1;
    
    Serial.println("Returning to main UI from full screen chart");
    // Note: Actual UI recreation should be handled by the main UI manager
}

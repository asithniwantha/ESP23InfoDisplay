#include "ui/UITouchHandler.h"
#include "UIConfig.h"
#include <Arduino.h>

UITouchHandler::UITouchHandler() {}

void UITouchHandler::handleTouch(int x, int y) {
    using namespace TouchZones;
    
    if (y >= CARD_Y_MIN && y <= CARD_Y_MAX) {
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
}

void UITouchHandler::handleVolumeTouch(int x, int y, SystemData& data) {
    int local_x = x - UILayout::VOLUME_CONTROL_X;
    if (local_x >= 25 && local_x <= UILayout::BOTTOM_CARD_WIDTH - 15) {
        int slider_width = UILayout::BOTTOM_CARD_WIDTH - 40;
        float volume_percent = ((local_x - 25) * 100.0f) / slider_width;
        volume_percent = constrain(volume_percent, 0, 100);
        
        data.setVolume((int)volume_percent);
        // This part of the original code updated the volume slider directly.
        // In the new architecture, this should be handled by the UIDataUpdater.
        // For now, we'll just set the data.
    }
}
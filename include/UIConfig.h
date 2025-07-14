#ifndef UI_CONFIG_H
#define UI_CONFIG_H

#include <lvgl.h>
#include "Defines.h"  // Include existing defines

// Network Configuration
namespace NetworkConfig {
    constexpr int UDP_PORT = 8080;
    constexpr const char* MDNS_HOSTNAME = "esp32display";
}

// UI Layout Constants
namespace UILayout {
    // Use screen dimensions from Defines.h
    // constexpr int SCREEN_WIDTH = 320;  // Now comes from Defines.h
    // constexpr int SCREEN_HEIGHT = 240; // Now comes from Defines.h
    
    // Status bar
    constexpr int STATUS_BAR_HEIGHT = 20;
    
    // System cards
    constexpr int CARD_WIDTH = 75;
    constexpr int CARD_HEIGHT = 35;
    constexpr int CARD_SPACING = 5;
    constexpr int CARD_START_X = 5;
    constexpr int CARD_START_Y = 25;
    constexpr int CARD_END_Y = CARD_START_Y + CARD_HEIGHT;
    
    // Progress bars
    constexpr int PROGRESS_BAR_WIDTH = 75;
    constexpr int PROGRESS_BAR_HEIGHT = 8;
    constexpr int PROGRESS_BAR_Y = 65;
    
    // Info cards
    constexpr int INFO_CARD_WIDTH = 120;
    constexpr int INFO_CARD_HEIGHT = 30;
    constexpr int INFO_CARD_SPACING = 8;
    constexpr int INFO_CARD_Y = 85;
    
    // Charts
    constexpr int CHART_WIDTH = 255;
    constexpr int CHART_HEIGHT = 25;
    constexpr int CHART_SPACING = 32;
    constexpr int CHART_START_Y = 125;
    constexpr int CHART_POINT_COUNT = 50;
    constexpr int CHART_ICON_OFFSET = 8;
}

// Touch zones
namespace TouchZones {
    // Card touch areas
    constexpr int CPU_CARD_X_MIN = UILayout::CARD_START_X;
    constexpr int CPU_CARD_X_MAX = CPU_CARD_X_MIN + UILayout::CARD_WIDTH;
    
    constexpr int RAM_CARD_X_MIN = CPU_CARD_X_MAX + UILayout::CARD_SPACING;
    constexpr int RAM_CARD_X_MAX = RAM_CARD_X_MIN + UILayout::CARD_WIDTH;
    
    constexpr int DISK_CARD_X_MIN = RAM_CARD_X_MAX + UILayout::CARD_SPACING;
    constexpr int DISK_CARD_X_MAX = DISK_CARD_X_MIN + UILayout::CARD_WIDTH;
    
    constexpr int TEMP_CARD_X_MIN = DISK_CARD_X_MAX + UILayout::CARD_SPACING;
    constexpr int TEMP_CARD_X_MAX = TEMP_CARD_X_MIN + UILayout::CARD_WIDTH;
    
    // Chart touch areas
    constexpr int CPU_CHART_Y_MIN = UILayout::CHART_START_Y;
    constexpr int CPU_CHART_Y_MAX = CPU_CHART_Y_MIN + UILayout::CHART_HEIGHT;
    
    constexpr int RAM_CHART_Y_MIN = CPU_CHART_Y_MAX + 7;
    constexpr int RAM_CHART_Y_MAX = RAM_CHART_Y_MIN + UILayout::CHART_HEIGHT;
    
    constexpr int TEMP_CHART_Y_MIN = RAM_CHART_Y_MAX + 7;
    constexpr int TEMP_CHART_Y_MAX = TEMP_CHART_Y_MIN + UILayout::CHART_HEIGHT;
}

// System thresholds
namespace SystemThresholds {
    constexpr float CPU_UPDATE_THRESHOLD = 0.5f;
    constexpr float RAM_UPDATE_THRESHOLD = 0.5f;
    constexpr float DISK_UPDATE_THRESHOLD = 0.1f;
    constexpr float NETWORK_UPDATE_THRESHOLD = 0.01f;
    
    constexpr int WARNING_LEVEL = 60;
    constexpr int DANGER_LEVEL = 80;
    constexpr int TEMP_WARNING_LEVEL = 50;
    constexpr int TEMP_DANGER_LEVEL = 70;
    
    constexpr unsigned long FORCE_UPDATE_INTERVAL = 5000; // 5 seconds
}

// Chart types enum
enum class ChartType {
    CPU = 0,
    RAM = 1,
    TEMPERATURE = 2,
    DISK = 3
};

// Colors
namespace UIColors {
    constexpr uint32_t PRIMARY = 0x2196F3;   // Blue
    constexpr uint32_t SUCCESS = 0x4CAF50;   // Green
    constexpr uint32_t WARNING = 0xFF9800;   // Orange
    constexpr uint32_t DANGER = 0xF44336;    // Red
    constexpr uint32_t INFO = 0x00BCD4;      // Cyan
    constexpr uint32_t DARK = 0x1A1A1A;      // Dark background
    constexpr uint32_t CARD = 0x2D2D2D;      // Card background
    constexpr uint32_t TEXT = 0xFFFFFF;      // White text
    constexpr uint32_t MUTED = 0x888888;     // Gray text
}

#endif // UI_CONFIG_H

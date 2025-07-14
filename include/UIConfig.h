#ifndef UI_CONFIG_H
#define UI_CONFIG_H

#include <lvgl.h>
#include "Defines.h"  // Include existing defines

// Network Configuration
namespace NetworkConfig {
    constexpr int UDP_PORT = 8080;
    constexpr const char* MDNS_HOSTNAME = "esp32display";
}

// UI Layout Constants - Landscape Mode (320x240)
namespace UILayout {
    // Use screen dimensions from Defines.h
    // constexpr int SCREEN_WIDTH = 320;  // Now comes from Defines.h
    // constexpr int SCREEN_HEIGHT = 240; // Now comes from Defines.h
    
    // Main layout
    constexpr int PADDING = 4;
    constexpr int GAP = 4;
    
    // Top row - 4 metric cards (adjusted to fit 320px width)
    constexpr int CARD_WIDTH = 72;   // Reduced from 75 to fit better
    constexpr int CARD_HEIGHT = 58;  // Reduced to fit in 240px height
    constexpr int CARD_ROW_Y = PADDING;
    
    // CPU history chart (properly sized to avoid overlap)
    constexpr int CPU_CHART_Y = CARD_ROW_Y + CARD_HEIGHT + GAP;
    constexpr int CPU_CHART_WIDTH = 320 - (PADDING * 2);
    constexpr int CPU_CHART_HEIGHT = 80;  // Reduced to prevent overlap
    
    // Bottom row - Network and Volume (moved to very bottom)
    constexpr int BOTTOM_CARD_HEIGHT = 50;  // Fixed height for bottom cards
    constexpr int BOTTOM_ROW_Y = 240 - BOTTOM_CARD_HEIGHT - PADDING;  // Position at bottom
    constexpr int BOTTOM_CARD_WIDTH = (320 - (PADDING * 3)) / 2;  // = 154px each
    
    // Network chart (left)
    constexpr int NETWORK_CHART_X = PADDING;
    constexpr int NETWORK_CHART_Y = BOTTOM_ROW_Y;
    
    // Volume control (right)
    constexpr int VOLUME_CONTROL_X = PADDING + BOTTOM_CARD_WIDTH + GAP;
    constexpr int VOLUME_CONTROL_Y = BOTTOM_ROW_Y;
    
    // Chart configurations
    constexpr int CPU_CHART_POINTS = 80;
    constexpr int NETWORK_CHART_POINTS = 40;
    
    // Temperature gauge (reduced size for smaller cards)
    constexpr int TEMP_GAUGE_SIZE = 45;
}

// Touch zones - Landscape Layout
namespace TouchZones {
    // Top row card positions (x positions only, y is same for all)
    constexpr int CARD_Y_MIN = UILayout::CARD_ROW_Y;
    constexpr int CARD_Y_MAX = CARD_Y_MIN + UILayout::CARD_HEIGHT;
    
    // CPU card (position 0)
    constexpr int CPU_CARD_X_MIN = UILayout::PADDING;
    constexpr int CPU_CARD_X_MAX = CPU_CARD_X_MIN + UILayout::CARD_WIDTH;
    
    // RAM card (position 1)
    constexpr int RAM_CARD_X_MIN = CPU_CARD_X_MAX + UILayout::GAP;
    constexpr int RAM_CARD_X_MAX = RAM_CARD_X_MIN + UILayout::CARD_WIDTH;
    
    // Temperature card (position 2)
    constexpr int TEMP_CARD_X_MIN = RAM_CARD_X_MAX + UILayout::GAP;
    constexpr int TEMP_CARD_X_MAX = TEMP_CARD_X_MIN + UILayout::CARD_WIDTH;
    
    // Disk card (position 3)
    constexpr int DISK_CARD_X_MIN = TEMP_CARD_X_MAX + UILayout::GAP;
    constexpr int DISK_CARD_X_MAX = DISK_CARD_X_MIN + UILayout::CARD_WIDTH;
    
    // CPU chart touch area
    constexpr int CPU_CHART_X_MIN = UILayout::PADDING;
    constexpr int CPU_CHART_X_MAX = CPU_CHART_X_MIN + UILayout::CPU_CHART_WIDTH;
    constexpr int CPU_CHART_Y_MIN = UILayout::CPU_CHART_Y;
    constexpr int CPU_CHART_Y_MAX = CPU_CHART_Y_MIN + UILayout::CPU_CHART_HEIGHT;
    
    // Network chart touch area
    constexpr int NETWORK_CHART_X_MIN = UILayout::NETWORK_CHART_X;
    constexpr int NETWORK_CHART_X_MAX = NETWORK_CHART_X_MIN + UILayout::BOTTOM_CARD_WIDTH;
    constexpr int NETWORK_CHART_Y_MIN = UILayout::NETWORK_CHART_Y;
    constexpr int NETWORK_CHART_Y_MAX = NETWORK_CHART_Y_MIN + UILayout::BOTTOM_CARD_HEIGHT;
    
    // Volume control touch area
    constexpr int VOLUME_CONTROL_X_MIN = UILayout::VOLUME_CONTROL_X;
    constexpr int VOLUME_CONTROL_X_MAX = VOLUME_CONTROL_X_MIN + UILayout::BOTTOM_CARD_WIDTH;
    constexpr int VOLUME_CONTROL_Y_MIN = UILayout::VOLUME_CONTROL_Y;
    constexpr int VOLUME_CONTROL_Y_MAX = VOLUME_CONTROL_Y_MIN + UILayout::BOTTOM_CARD_HEIGHT;
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

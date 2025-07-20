#ifndef LVGL_TOUCH_H
#define LVGL_TOUCH_H

#include "SystemData.h"
#include "UIConfig.h"

class LVGLTouch {
public:
    LVGLTouch();
    ~LVGLTouch();
    
    void handleTouch(int x, int y);
    void showTouchDebug(int x, int y, int z);
    
    // Touch handlers for specific components
    void handleVolumeTouch(int x, int y, SystemData& data);
    
    // Full screen chart functionality
    void showFullScreenChart(int chartType);
    void returnToMainUI();
    bool isInFullScreenMode() const { return isFullScreenMode; }
    
private:
    // Full screen chart state
    bool isFullScreenMode = false;
    int currentFullScreenChart = -1; // -1: none, others use ChartType enum
};

#endif // LVGL_TOUCH_H

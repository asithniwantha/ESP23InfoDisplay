#pragma once
#include "SystemData.h"

class UITouchHandler {
public:
    UITouchHandler();
    void handleTouch(int x, int y);
    void handleVolumeTouch(int x, int y, SystemData& data);
};

#pragma once
#include <XPT2046_Touchscreen.h>
#include <TFT_eSPI.h>
#include "Defines.h"

class TouchManager {
public:
    TouchManager(uint8_t cs_pin, uint8_t irq_pin);
    void begin();
    bool isTouched();
    TS_Point getPoint();
    void calibrate(TS_Point &p);
    void printToSerial(int x, int y, int z);

private:
    XPT2046_Touchscreen ts;
    uint8_t _irq_pin;
};

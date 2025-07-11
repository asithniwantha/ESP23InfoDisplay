#ifndef TOUCH_MANAGER_H
#define TOUCH_MANAGER_H

#include <XPT2046_Touchscreen.h>
#include <SPI.h>
#include "Defines.h"

class TouchManager {
public:
    TouchManager(uint8_t csPin, uint8_t irqPin);
    void begin();
    bool isTouched();
    TS_Point getPoint();
    void calibrate(TS_Point& p);
    void printToSerial(int x, int y, int z);

private:
    XPT2046_Touchscreen touchscreen;
    SPIClass spi;
};

#endif // TOUCH_MANAGER_H

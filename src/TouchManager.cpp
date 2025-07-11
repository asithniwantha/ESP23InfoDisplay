#include "TouchManager.h"

TouchManager::TouchManager(uint8_t csPin, uint8_t irqPin)
    : touchscreen(csPin, irqPin), spi(VSPI) {}

void TouchManager::begin() {
    spi.begin(XPT2046_CLK, XPT2046_MISO, XPT2046_MOSI, XPT2046_CS);
    touchscreen.begin(spi);
    touchscreen.setRotation(1);
}

bool TouchManager::isTouched() {
    return touchscreen.tirqTouched() && touchscreen.touched();
}

TS_Point TouchManager::getPoint() {
    return touchscreen.getPoint();
}

void TouchManager::calibrate(TS_Point& p) {
    p.x = map(p.x, 200, 3700, 1, SCREEN_WIDTH);
    p.y = map(p.y, 240, 3800, 1, SCREEN_HEIGHT);
}

void TouchManager::printToSerial(int x, int y, int z) {
    Serial.print("X = ");
    Serial.print(x);
    Serial.print(" | Y = ");
    Serial.print(y);
    Serial.print(" | Pressure = ");
    Serial.print(z);
    Serial.println();
}

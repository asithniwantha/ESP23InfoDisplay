#include "TouchManager.h"

TouchManager::TouchManager(uint8_t cs_pin, uint8_t irq_pin) : ts(cs_pin), _irq_pin(irq_pin) {}

void TouchManager::begin() {
    ts.begin();
    ts.setRotation(1);
}

bool TouchManager::isTouched() {
    return ts.touched();
}

TS_Point TouchManager::getPoint() {
    return ts.getPoint();
}

void TouchManager::calibrate(TS_Point &p) {
    // This is a placeholder for a proper calibration function
    // For now, we just swap x and y
    int x = p.x;
    p.x = p.y;
    p.y = x;
}

void TouchManager::printToSerial(int x, int y, int z) {
    Serial.printf("Touch: X=%d, Y=%d, Z=%d\n", x, y, z);
}


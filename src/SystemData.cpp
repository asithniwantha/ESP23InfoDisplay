#include "SystemData.h"

SystemData::SystemData() : historyIndex(0), historyFull(false) {
    for (int i = 0; i < HISTORY_SIZE; i++) {
        cpuHistory[i] = 0;
        ramHistory[i] = 0;
        tempHistory[i] = 0;
    }
}

void SystemData::update(String jsonData) {
    DynamicJsonDocument doc(1024);
    DeserializationError error = deserializeJson(doc, jsonData);

    if (error) {
        Serial.print("JSON parsing failed: ");
        Serial.println(error.c_str());
        return;
    }

    currentData.cpuUsage = doc["CpuUsage"];
    currentData.ramUsage = doc["RamUsage"];
    currentData.diskUsage = doc["DiskUsage"];
    currentData.networkSpeed = doc["NetworkSpeed"];
    currentData.volumeLevel = doc["VolumeLevel"];
    currentData.cpuTemp = doc["CpuTemperature"];
    
    String timestamp = doc["Timestamp"];
    int timeStart = timestamp.indexOf('T') + 1;
    int timeEnd = timestamp.indexOf('.');
    if (timeStart > 0 && timeEnd > timeStart) {
        currentData.lastTime = timestamp.substring(timeStart, timeEnd);
    }

    addToHistory(currentData.cpuUsage, currentData.ramUsage, currentData.cpuTemp);
}

void SystemData::addToHistory(float cpu, float ram, float temp) {
    cpuHistory[historyIndex] = cpu;
    ramHistory[historyIndex] = ram;
    tempHistory[historyIndex] = temp;
    
    historyIndex = (historyIndex + 1) % HISTORY_SIZE;
    if (historyIndex == 0) {
        historyFull = true;
    }
}

void SystemData::reset() {
    currentData.initialized = false;
    currentData.cpuUsage = -1;
    currentData.ramUsage = -1;
    currentData.diskUsage = -1;
    currentData.networkSpeed = -1;
    currentData.volumeLevel = -1;
    currentData.cpuTemp = -1;
    currentData.lastTime = "";
    historyIndex = 0;
    historyFull = false;
    
    for (int i = 0; i < HISTORY_SIZE; i++) {
        cpuHistory[i] = 0;
        ramHistory[i] = 0;
        tempHistory[i] = 0;
    }
}

bool SystemData::hasSignificantChange(float newCpu, float newRam, int newTemp) {
    return (!currentData.initialized || 
            abs(newCpu - currentData.cpuUsage) > 0.5 ||
            abs(newRam - currentData.ramUsage) > 0.5 ||
            newTemp != currentData.cpuTemp);
}

SystemStats& SystemData::getCurrentData() {
    return currentData;
}

float* SystemData::getCpuHistory() {
    return cpuHistory;
}

float* SystemData::getRamHistory() {
    return ramHistory;
}

float* SystemData::getTempHistory() {
    return tempHistory;
}

int SystemData::getHistoryIndex() {
    return historyIndex;
}

bool SystemData::isHistoryFull() {
    return historyFull;
}

void SystemData::getScrollingData(float* destArray, float* sourceArray, int maxPoints) {
    // This creates a scrolling effect by arranging data chronologically
    // Newest data appears on the right, oldest on the left
    
    int pointsToShow = getDataPointsToShow();
    if (pointsToShow > maxPoints) pointsToShow = maxPoints;
    
    if (!historyFull) {
        // Linear array from 0 to historyIndex
        for (int i = 0; i < pointsToShow; i++) {
            destArray[i] = sourceArray[i];
        }
    } else {
        // Circular buffer - need to arrange in chronological order
        // historyIndex points to the oldest data
        for (int i = 0; i < pointsToShow; i++) {
            int sourceIndex = (historyIndex + i) % HISTORY_SIZE;
            destArray[i] = sourceArray[sourceIndex];
        }
    }
}

int SystemData::getDataPointsToShow() {
    return historyFull ? HISTORY_SIZE : historyIndex;
}

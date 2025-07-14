#ifndef SYSTEM_DATA_H
#define SYSTEM_DATA_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include "Defines.h"

struct SystemStats {
  float cpuUsage = -1;
  float ramUsage = -1;
  float diskUsage = -1;
  float networkSpeed = -1;
  int volumeLevel = -1;
  int cpuTemp = -1;
  String lastTime = "";
  bool initialized = false;
};

class SystemData {
public:
    SystemData();
    void update(String jsonData);
    void addToHistory(float cpu, float ram, float temp);
    void reset();
    bool hasSignificantChange(float newCpu, float newRam, int newTemp);
    
    SystemStats &getCurrentData();
    float* getCpuHistory();
    float* getRamHistory();
    float* getTempHistory();
    int getHistoryIndex();
    bool isHistoryFull();
    
    // New methods for scrolling graphs
    void getScrollingData(float* destArray, float* sourceArray, int maxPoints);
    int getDataPointsToShow();
    
    // Helper method for formatting network speed
    String getFormattedNetworkSpeed();
    
    // Volume control methods
    void setVolume(int volume);
    int getVolume() const;
    String getFormattedVolume() const;

private:
    SystemStats currentData;
    float cpuHistory[HISTORY_SIZE];
    float ramHistory[HISTORY_SIZE];
    float tempHistory[HISTORY_SIZE];
    int historyIndex;
    bool historyFull;
};

#endif // SYSTEM_DATA_H

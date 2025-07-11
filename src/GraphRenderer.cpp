#include "GraphRenderer.h"

GraphRenderer::GraphRenderer(TFT_eSPI& display) : tft(display) {}

void GraphRenderer::drawScrollingGraph(int x, int y, int width, int height, SystemData& data, int graphType, uint16_t color) {
    // Graph types: 0=CPU, 1=RAM, 2=TEMP
    const int maxPoints = width - 40; // Leave space for value display on the right
    float graphData[maxPoints];
    
    // Get the data in scrolling order (oldest to newest)
    switch(graphType) {
        case 0: data.getScrollingData(graphData, data.getCpuHistory(), maxPoints); break;
        case 1: data.getScrollingData(graphData, data.getRamHistory(), maxPoints); break;
        case 2: data.getScrollingData(graphData, data.getTempHistory(), maxPoints); break;
        default: return;
    }
    
    int pointsToShow = data.getDataPointsToShow();
    if (pointsToShow > maxPoints) pointsToShow = maxPoints;
    if (pointsToShow < 2) return;
    
    // Clear the graph area
    tft.fillRect(x, y, width, height, COLOR_BG);
    tft.drawRect(x, y, width - 35, height, COLOR_MUTED); // Leave space for current value
    
    // Find min/max for scaling (use appropriate ranges for each graph type)
    float minVal, maxVal;
    if (graphType == 2) { // Temperature
        minVal = 20; maxVal = 90; // Fixed range for temperature
    } else { // CPU/RAM percentages
        minVal = 0; maxVal = 100; // Fixed 0-100% range
    }
    
    // Adaptive scaling if data goes outside fixed ranges
    for (int i = 0; i < pointsToShow; i++) {
        if (graphData[i] < minVal) minVal = graphData[i] - 5;
        if (graphData[i] > maxVal) maxVal = graphData[i] + 5;
    }
    
    if (maxVal - minVal < 1) maxVal = minVal + 1;
    
    // Draw the scrolling graph - spread across full width
    int graphWidth = width - 40; // Graph area width
    int startX = x + 1;
    
    // Calculate pixels per point to spread across full graph width
    float pixelsPerPoint = (float)(graphWidth - 2) / (pointsToShow - 1);
    if (pixelsPerPoint < 1.0f) pixelsPerPoint = 1.0f;
    
    // Draw connecting lines between points with better smoothing
    uint16_t fadeColor = color;
    for (int i = 1; i < pointsToShow; i++) {
        int x1 = startX + (int)((i - 1) * pixelsPerPoint);
        int y1 = y + height - 1 - ((graphData[i - 1] - minVal) * (height - 2)) / (maxVal - minVal);
        int x2 = startX + (int)(i * pixelsPerPoint);
        int y2 = y + height - 1 - ((graphData[i] - minVal) * (height - 2)) / (maxVal - minVal);
        
        // Ensure points stay within bounds
        if (x1 >= startX && x2 < x + graphWidth - 5) {
            // Draw line with slight fade effect for older data
            if (i < pointsToShow / 4) {
                fadeColor = (color & 0xE79C) >> 1; // Fade older data
            } else {
                fadeColor = color;
            }
            
            tft.drawLine(x1, y1, x2, y2, fadeColor);
            
            // Draw emphasis on recent data points
            if (i >= pointsToShow - 3) {
                tft.drawPixel(x2, y2, color);
                // Make the newest point more prominent
                if (i == pointsToShow - 1) {
                    if (y2 > y && y2 < y + height - 1) {
                        tft.drawPixel(x2 - 1, y2, color);
                        tft.drawPixel(x2 + 1, y2, color);
                    }
                }
            }
        }
    }
    
    // Draw grid lines for better readability (every 25%)
    if (graphType != 2) { // Only for percentage graphs
        for (int pct = 25; pct <= 75; pct += 25) {
            int gridY = y + height - 1 - ((pct - minVal) * (height - 2)) / (maxVal - minVal);
            if (gridY > y && gridY < y + height - 1) {
                for (int gx = x + 1; gx < x + graphWidth - 5; gx += 8) {
                    tft.drawPixel(gx, gridY, COLOR_MUTED);
                }
            }
        }
    }
    
    // Display current value on the right side
    SystemStats& stats = data.getCurrentData();
    String currentValue;
    switch(graphType) {
        case 0: currentValue = String(stats.cpuUsage, 1) + "%"; break;
        case 1: currentValue = String(stats.ramUsage, 1) + "%"; break;
        case 2: currentValue = String(stats.cpuTemp) + "°"; break;
    }
    
    tft.setTextColor(color, COLOR_BG);
    tft.drawString(currentValue, x + width - 32, y + (height - 8) / 2, 1);
}

void GraphRenderer::drawMiniGraph(int x, int y, int width, int height, float* data, int dataSize, uint16_t color) {
    tft.fillRect(x, y, width, height, COLOR_BG);
    tft.drawRect(x, y, width, height, COLOR_MUTED);
    
    if (dataSize < 2) return;
    
    float minVal = data[0], maxVal = data[0];
    for (int i = 0; i < dataSize; i++) {
        if (data[i] < minVal) minVal = data[i];
        if (data[i] > maxVal) maxVal = data[i];
    }
    
    if (maxVal - minVal < 1) maxVal = minVal + 1;
    
    for (int i = 1; i < dataSize; i++) {
        int x1 = x + 1 + ((i - 1) * (width - 2)) / (dataSize - 1);
        int y1 = y + height - 1 - ((data[i - 1] - minVal) * (height - 2)) / (maxVal - minVal);
        int x2 = x + 1 + (i * (width - 2)) / (dataSize - 1);
        int y2 = y + height - 1 - ((data[i] - minVal) * (height - 2)) / (maxVal - minVal);

        tft.drawLine(x1, y1, x2, y2, color);
    }
}

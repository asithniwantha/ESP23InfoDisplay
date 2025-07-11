#include "ScreenManager.h"

ScreenManager::ScreenManager(TFT_eSPI& display, UIRenderer& uiRenderer) 
    : tft(display), ui(uiRenderer) {}

void ScreenManager::showStartupScreen() {
    tft.fillScreen(COLOR_BG);
    ui.drawStatusBar();

    int centerX = SCREEN_WIDTH / 2;
    int centerY = SCREEN_HEIGHT / 2;

    tft.fillRoundRect(30, centerY - 40, SCREEN_WIDTH - 60, 80, 8, COLOR_CARD);
    tft.drawRoundRect(30, centerY - 40, SCREEN_WIDTH - 60, 80, 8, COLOR_PRIMARY);

    tft.setTextColor(COLOR_PRIMARY, COLOR_CARD);
    tft.drawCentreString("ESP32 System Monitor", centerX, centerY - 20, 2);

    tft.setTextColor(COLOR_TEXT, COLOR_CARD);
    tft.drawCentreString("Connecting to WiFi...", centerX, centerY + 5, 1);

    for (int i = 0; i < 100; i += 5) {
        ui.drawProgressBar(50, centerY + 20, SCREEN_WIDTH - 100, 8, i, COLOR_PRIMARY);
        delay(50);
    }
}

void ScreenManager::displayWiFiInfo(String ip, String mdns, int port) {
    tft.fillScreen(COLOR_BG);
    ui.drawStatusBar();

    int centerX = SCREEN_WIDTH / 2;
    int startY = 40;

    tft.fillRoundRect(20, startY, SCREEN_WIDTH - 40, 160, 8, COLOR_CARD);
    tft.drawRoundRect(20, startY, SCREEN_WIDTH - 40, 160, 8, COLOR_SUCCESS);

    tft.fillCircle(centerX, startY + 30, 8, COLOR_SUCCESS);
    tft.setTextColor(COLOR_SUCCESS, COLOR_CARD);
    tft.drawCentreString("WiFi Connected!", centerX, startY + 50, 2);

    tft.setTextColor(COLOR_TEXT, COLOR_CARD);
    tft.drawCentreString("IP: " + ip, centerX, startY + 75, 1);
    tft.drawCentreString("mDNS: " + mdns + ".local", centerX, startY + 95, 1);
    tft.drawCentreString("UDP Port: " + String(port), centerX, startY + 115, 1);

    tft.setTextColor(COLOR_PRIMARY, COLOR_CARD);
    tft.drawCentreString("Ready for data...", centerX, startY + 140, 2);

    delay(3000);
}

void ScreenManager::displayUDPText(String text) {
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);

    int centerX = SCREEN_WIDTH / 2;
    tft.drawCentreString("UDP Message:", centerX, 20, FONT_SIZE);

    int maxCharsPerLine = 25;
    int currentY = 60;

    for (int i = 0; i < text.length(); i += maxCharsPerLine) {
        String line = text.substring(i, min(i + maxCharsPerLine, (int)text.length()));
        tft.drawCentreString(line, centerX, currentY, FONT_SIZE);
        currentY += 25;
        if (currentY > SCREEN_HEIGHT - 30) break;
    }
}

void ScreenManager::printTouchToDisplay(int x, int y, int z) {
    tft.fillScreen(TFT_WHITE);
    tft.setTextColor(TFT_BLACK, TFT_WHITE);

    int centerX = SCREEN_WIDTH / 2;
    int textY = 80;

    String tempText = "X = " + String(x);
    tft.drawCentreString(tempText, centerX, textY, FONT_SIZE);

    textY += 20;
    tempText = "Y = " + String(y);
    tft.drawCentreString(tempText, centerX, textY, FONT_SIZE);

    textY += 20;
    tempText = "Pressure = " + String(z);
    tft.drawCentreString(tempText, centerX, textY, FONT_SIZE);
}

#include "ScreenManager.h"
#include <TFT_eSPI.h>
#include "Defines.h"

ScreenManager::ScreenManager(TFT_eSPI& display, UIRenderer& uiRenderer) 
    : tft(display), ui(uiRenderer) {}

void ScreenManager::showStartupScreen() {
    tft.fillScreen(COLOR_BG);
    ui.drawStatusBar();

    Serial.println("[DEBUG] Entered ScreenManager::showStartupScreen()");
    int centerX = tft.width() / 2;
    int centerY = tft.height() / 2;

    tft.fillRoundRect(30, centerY - 40, tft.width() - 60, 80, 8, COLOR_CARD);
    tft.drawRoundRect(30, centerY - 40, tft.width() - 60, 80, 8, COLOR_PRIMARY);

    tft.setTextColor(COLOR_PRIMARY, COLOR_CARD);
    tft.drawCentreString("ESP32 System Monitor", centerX, centerY - 20, 2);

    tft.setTextColor(COLOR_TEXT, COLOR_CARD);
    tft.drawCentreString("Connecting to WiFi...", centerX, centerY + 5, 1);

    for (int i = 0; i < 100; i += 5) {
        ui.drawProgressBar(50, centerY + 20, tft.width() - 100, 8, i, COLOR_PRIMARY);
        delay(50);
    }
    Serial.println("[DEBUG] Leaving ScreenManager::showStartupScreen()");
}

void ScreenManager::displayWiFiInfo(String ip, String mdns, int port) {
    Serial.println("[DEBUG] Entered ScreenManager::displayWiFiInfo()");
    tft.fillScreen(COLOR_BG);
    ui.drawStatusBar();
    Serial.println("[DEBUG] After drawStatusBar()");

    int centerX = tft.width() / 2;
    int startY = 40;
    Serial.println("[DEBUG] Calculated centerX and startY");

    tft.fillRoundRect(20, startY, tft.width() - 40, 160, 8, COLOR_CARD);
    tft.drawRoundRect(20, startY, tft.width() - 40, 160, 8, COLOR_SUCCESS);
    Serial.println("[DEBUG] Drew WiFi info card background");

    tft.fillCircle(centerX, startY + 30, 8, COLOR_SUCCESS);
    tft.setTextColor(COLOR_SUCCESS, COLOR_CARD);
    tft.drawCentreString("WiFi Connected!", centerX, startY + 50, 2);
    Serial.println("[DEBUG] Drew WiFi connected text");

    tft.setTextColor(COLOR_TEXT, COLOR_CARD);
    tft.drawCentreString("IP: " + ip, centerX, startY + 75, 1);
    tft.drawCentreString("mDNS: " + mdns + ".local", centerX, startY + 95, 1);
    tft.drawCentreString("UDP Port: " + String(port), centerX, startY + 115, 1);
    Serial.println("[DEBUG] Drew IP, mDNS, UDP Port");

    tft.setTextColor(COLOR_PRIMARY, COLOR_CARD);
    tft.drawCentreString("Ready for data...", centerX, startY + 140, 2);
    Serial.println("[DEBUG] Drew ready for data text");

    delay(3000);
    Serial.println("[DEBUG] Leaving ScreenManager::displayWiFiInfo()");
}

void ScreenManager::displayUDPText(String text) {
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);

    int centerX = tft.width() / 2;
    tft.drawCentreString("UDP Message:", centerX, 20, FONT_SIZE);

    int maxCharsPerLine = 25;
    int currentY = 60;

    for (int i = 0; i < text.length(); i += maxCharsPerLine) {
        String line = text.substring(i, min(i + maxCharsPerLine, (int)text.length()));
        tft.drawCentreString(line, centerX, currentY, FONT_SIZE);
        currentY += 25;
        if (currentY > tft.height() - 30) break;
    }
}

void ScreenManager::printTouchToDisplay(int x, int y, int z) {
    tft.fillScreen(TFT_WHITE);
    tft.setTextColor(TFT_BLACK, TFT_WHITE);

    int centerX = tft.width() / 2;
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
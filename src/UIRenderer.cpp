#include "UIRenderer.h"

UIRenderer::UIRenderer(TFT_eSPI& display) : tft(display) {}

void UIRenderer::drawProgressBar(int x, int y, int width, int height, float percentage, uint16_t color) {
    tft.fillRoundRect(x, y, width, height, 2, COLOR_BG);
    tft.drawRoundRect(x, y, width, height, 2, COLOR_MUTED);
    
    if (percentage > 0) {
        int fillWidth = (width - 4) * (percentage / 100.0);
        if (fillWidth > 0) {
            tft.fillRoundRect(x + 2, y + 2, fillWidth, height - 4, 1, color);
        }
    }
    
    String percentText = String(percentage, 1) + "%";
    int textWidth = percentText.length() * 6;
    int textX = x + (width - textWidth) / 2;
    int textY = y + (height - 8) / 2;
    tft.setTextColor(COLOR_TEXT, 0);
    tft.drawString(percentText, textX, textY, 1);
}

void UIRenderer::drawCard(int x, int y, int width, int height, String title, String value, uint16_t color) {
    tft.fillRoundRect(x, y, width, height, 4, COLOR_CARD);
    tft.drawRoundRect(x, y, width, height, 4, color);
    
    tft.setTextColor(COLOR_MUTED, COLOR_CARD);
    tft.drawString(title, x + 5, y + 3, 1);
    
    tft.setTextColor(color, COLOR_CARD);
    tft.drawString(value, x + 5, y + 15, 2);
}

void UIRenderer::drawStatusBar() {
    tft.fillRect(0, 0, SCREEN_WIDTH, 20, COLOR_PRIMARY);
    
    tft.setTextColor(COLOR_TEXT, COLOR_PRIMARY);
    tft.drawString("WiFi", 5, 6, 1);
    
    String mdnsAddr = "esp32display.local";
    int textWidth = mdnsAddr.length() * 6;
    tft.drawString(mdnsAddr, SCREEN_WIDTH - textWidth - 5, 6, 1);
}

void UIRenderer::clearArea(int x, int y, int width, int height) {
    tft.fillRect(x, y, width, height, COLOR_BG);
}

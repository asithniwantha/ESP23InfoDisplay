#ifndef UI_RENDERER_H
#define UI_RENDERER_H

#include <TFT_eSPI.h>
#include "Defines.h"

class UIRenderer {
public:
    UIRenderer(TFT_eSPI& display);
    
    void drawProgressBar(int x, int y, int width, int height, float percentage, uint16_t color);
    void drawCard(int x, int y, int width, int height, String title, String value, uint16_t color);
    void drawStatusBar();
    void clearArea(int x, int y, int width, int height);

private:
    TFT_eSPI& tft;
};

#endif // UI_RENDERER_H

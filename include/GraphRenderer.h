#ifndef GRAPH_RENDERER_H
#define GRAPH_RENDERER_H

#include <TFT_eSPI.h>
#include "SystemData.h"
#include "Defines.h"

class GraphRenderer {
public:
    GraphRenderer(TFT_eSPI& display);
    
    void drawScrollingGraph(int x, int y, int width, int height, SystemData& data, int graphType, uint16_t color);
    void drawMiniGraph(int x, int y, int width, int height, float* data, int dataSize, uint16_t color);

private:
    TFT_eSPI& tft;
};

#endif // GRAPH_RENDERER_H

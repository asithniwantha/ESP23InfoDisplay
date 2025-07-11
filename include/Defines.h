#ifndef DEFINES_H
#define DEFINES_H

#include <TFT_eSPI.h>

// Touchscreen pins
#define XPT2046_IRQ 36   // T_IRQ
#define XPT2046_MOSI 32  // T_DIN
#define XPT2046_MISO 39  // T_OUT
#define XPT2046_CLK 25   // T_CLK
#define XPT2046_CS 33    // T_CS

// Screen dimensions
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define FONT_SIZE 2

// History size for graphs
#define HISTORY_SIZE 20

// Colors for modern UI
#define COLOR_BG 0x0841      // Dark blue background
#define COLOR_CARD 0x1082    // Card background
#define COLOR_PRIMARY 0x07FF // Cyan
#define COLOR_SUCCESS 0x07E0 // Green
#define COLOR_WARNING 0xFFE0 // Yellow
#define COLOR_DANGER 0xF800  // Red
#define COLOR_INFO 0x867F    // Light blue
#define COLOR_TEXT 0xFFFF    // White
#define COLOR_MUTED 0x8410   // Gray

// Basic colors
#define BLACK 0x0000
#define WHITE 0xFFFF

// TFT_eSPI standard colors for compatibility
#define TFT_BLACK   0x0000
#define TFT_WHITE   0xFFFF
#define TFT_RED     0xF800
#define TFT_GREEN   0x07E0
#define TFT_BLUE    0x001F
#define TFT_CYAN    0x07FF
#define TFT_MAGENTA 0xF81F
#define TFT_YELLOW  0xFFE0

#endif // DEFINES_H

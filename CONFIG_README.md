# ESP32 CYD Configuration Guide

This project contains multiple configuration files for different ESP32 Cheap Yellow Display (CYD) board variants.

## Configuration Files

### `platformio.ini` (CURRENT - WORKING)
- **Display Driver**: ILI9341_2_DRIVER
- **Status**: ✅ **WORKING PERFECTLY**
- **Board Type**: Standard ESP32-2432S028R with single USB
- **Library Version**: TFT_eSPI@2.5.34 (stable)
- **Touch**: Fully functional with XPT2046_Touchscreen
- **Use this for**: Most standard CYD boards

### `platformio_alt.ini` (ALTERNATIVE)
- **Display Driver**: ST7789_DRIVER
- **Status**: ⚠️ **ALTERNATIVE CONFIG**
- **Board Type**: Dual USB CYD boards or ST7789 variants
- **Library Version**: TFT_eSPI@2.5.34 (stable)
- **Features**: Includes TFT_INVERSION_ON for color correction
- **Use this for**: 
  - Boards with dual USB ports (USB-C + micro USB)
  - Boards showing inverted colors with ILI9341
  - Boards with blank screen using ILI9341 driver

### `platformio_working_backup.ini`
- **Purpose**: Backup of the working configuration
- **Status**: ✅ **BACKUP OF WORKING CONFIG**
- **Use**: Restore if main config gets corrupted

## How to Switch Configurations

### To try the alternative configuration:
1. Rename `platformio.ini` to `platformio_ili9341.ini`
2. Rename `platformio_alt.ini` to `platformio.ini`
3. Build and upload: `pio run --target upload`

### To restore working configuration:
1. Rename current `platformio.ini` to `platformio_alt.ini`
2. Copy `platformio_working_backup.ini` to `platformio.ini`
3. Build and upload: `pio run --target upload`

## Troubleshooting

### If display is blank:
- Try switching from ILI9341 to ST7789 driver
- Check TFT_INVERSION_ON/OFF settings

### If colors are wrong:
- Try `TFT_INVERSION_ON=1` or `TFT_INVERSION_OFF=1`
- Check `TFT_RGB_ORDER` (TFT_BGR vs TFT_RGB)

### If touch doesn't work:
- Ensure XPT2046_Touchscreen library is included
- Check HSPI configuration with `USE_HSPI_PORT=1`
- Verify touch pin definitions

## Current Status
✅ **WORKING PERFECTLY** with `platformio.ini` (ILI9341_2_DRIVER configuration)
- Display: Working
- Touch: Working 
- Serial Output: Showing proper touch coordinates

**No changes needed unless you encounter issues!**

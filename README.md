# ESP32 System Monitor Display

A modern, real-time system monitoring display for ESP32 with touchscreen interface. Features Windows Task Manager-style scrolling graphs, anti-flicker updates, and a beautiful modern UI.

![ESP32 System Monitor](https://img.shields.io/badge/ESP32-System%20Monitor-blue.svg) ![Platform](https://img.shields.io/badge/Platform-ESP32-green.svg) ![License](https://img.shields.io/badge/License-MIT-yellow.svg)

## 🚀 Features

- **Real-time System Monitoring**: Live display of CPU, RAM, Disk usage, and Temperature
- **Windows Task Manager-style Graphs**: Smooth scrolling historical data visualization
- **Modern UI Design**: Clean cards, progress bars, and color-coded status indicators
- **Anti-flicker Technology**: Partial screen updates for smooth performance
- **Touch Interface**: Responsive touchscreen support with XPT2046
- **Network Integration**: WiFi connectivity with UDP data reception and mDNS discovery
- **Modular Architecture**: Clean, maintainable code structure with separated concerns

## 📱 Hardware Requirements

### Supported ESP32 Display Modules
- **ESP32-2432S028R** (Primary target - 2.8" 240×320 ILI9341 TFT)
- **ESP32 + ILI9341 TFT Display** (2.8" recommended)
- **XPT2046 Touch Controller** (integrated in most ESP32 display modules)

### Pin Configuration (ESP32-2432S028R)
```
TFT Display (ILI9341):
├── MISO: GPIO 12
├── MOSI: GPIO 13  
├── SCLK: GPIO 14
├── CS:   GPIO 15
├── DC:   GPIO 2
├── RST:  Not connected (-1)
└── BL:   GPIO 21

Touch Controller (XPT2046):
├── MOSI: GPIO 32
├── MISO: GPIO 39
├── SCLK: GPIO 25
├── CS:   GPIO 33
└── IRQ:  GPIO 36
```

## 🛠️ Software Requirements

- **PlatformIO** (recommended) or Arduino IDE
- **ESP32 Arduino Core** (v2.0.0+)
- **Libraries** (automatically installed via PlatformIO):
  - TFT_eSPI (v2.5.34)
  - XPT2046_Touchscreen
  - ArduinoJson (v6.21.3+)
  - WiFi (ESP32 built-in)
  - ESPmDNS (ESP32 built-in)

## 📦 Installation

### Method 1: PlatformIO (Recommended)

1. **Clone the repository:**
   ```bash
   git clone https://github.com/asithniwantha/ESP23InfoDisplay.git
   cd ESP23InfoDisplay
   ```

2. **Open in PlatformIO:**
   ```bash
   # Using VS Code with PlatformIO extension
   code .
   
   # Or using PlatformIO CLI
   pio run
   ```

3. **Upload to ESP32:**
   ```bash
   pio run --target upload
   ```

### Method 2: Arduino IDE

1. Download and extract the repository
2. Copy all files from `src/` to your Arduino sketch folder
3. Copy all files from `include/` to your sketch folder
4. Install required libraries through Library Manager
5. Configure TFT_eSPI for your display (see Configuration section)
6. Upload to ESP32

## ⚙️ Configuration

### WiFi Setup
Edit the network credentials in your data source or use the built-in WiFi manager:

```cpp
// The device will create an access point if no saved credentials
// Connect to "ESP32-Monitor" and configure via web interface
```

### Display Configuration
The project is pre-configured for ESP32-2432S028R. For other displays, modify `platformio.ini`:

```ini
build_flags = 
    -DUSER_SETUP_LOADED=1
    -DILI9341_2_DRIVER=1        ; Change for your display driver
    -DTFT_WIDTH=240             ; Your display width
    -DTFT_HEIGHT=320            ; Your display height
    # ... adjust pin definitions as needed
```

## 📊 Data Protocol

The system receives monitoring data via UDP on port 12345. Send JSON formatted data:

```json
{
  "cpu": 45.2,
  "ram": 67.8,
  "disk": 23.1,
  "temp": 52,
  "network": 2.5,
  "volume": 75,
  "time": "14:30:25"
}
```

**Network Speed Display:**
- Values < 1 MB/s: Displayed in KB/s (e.g., "256.5KB/s", "15KB/s")  
- Values ≥ 1 MB/s: Displayed in MB/s (e.g., "2.5MB/s", "15MB/s")
- Automatic unit selection for optimal readability

```

## 🎨 UI Layout & Display Elements

### Main Dashboard Layout
```
┌─────────────────────────────────────────────────┐
│ Status Bar (Time, WiFi, IP)                     │ Row 0
├─────────────────────────────────────────────────┤
│ [CPU] [RAM] [DISK] [TEMP]                      │ Row 1 (y=25)
│ [████] [███ ] [██  ] [█   ]                     │ Row 2 (y=65)
├─────────────────────────────────────────────────┤
│ [Net: 2.5MB/s] [Vol: 75%]  Updated: 14:30:25   │ Row 3 (y=85)
├─────────────────────────────────────────────────┤
│ 📊 CPU: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  │ Row 4 (y=125)
│ 🔹 RAM: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  │ Row 5 (y=157)  
│ 🌡️ TEMP:~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  │ Row 6 (y=189)
└─────────────────────────────────────────────────┘
```

### 📊 Display Elements Overview

#### **Status Bar (Top Row)**
- **Current Time**: Real-time clock display (HH:MM:SS format)
- **WiFi Status**: Connection indicator with signal strength
- **IP Address**: Current network IP for UDP data reception
- **mDNS Name**: Device hostname for network discovery

#### **System Metric Cards (75×35px each)**
| Card | Display | Color Coding | Range |
|------|---------|--------------|-------|
| **CPU** | Percentage usage | 🟢 <60% → 🟡 60-80% → 🔴 >80% | 0-100% |
| **RAM** | Memory utilization | 🟢 <60% → 🟡 60-80% → 🔴 >80% | 0-100% |
| **DISK** | Storage usage | 🟢 <60% → 🟡 60-80% → 🔴 >80% | 0-100% |
| **TEMP** | System temperature | 🟢 <50°C → 🟡 50-70°C → 🔴 >70°C | 0-100°C |

#### **Progress Bars (75×8px each)**
- Visual representation of each metric below the cards
- Animated fill with gradient colors
- Smooth transitions between states
- Real-time updates with anti-flicker technology

#### **Information Cards (120×30px each)**
| Card | Format | Examples | Auto-Scaling |
|------|--------|----------|--------------|
| **Network Speed** | Auto KB/s or MB/s | `156KB/s`, `2.5MB/s` | <1MB = KB/s, ≥1MB = MB/s |
| **Volume Level** | Percentage | `Vol: 75%`, `Vol: 0%` | 0-100% with mute detection |

#### **Real-time Charts (255×25px each)**
| Chart | Icon | Data Points | Update Rate | Color |
|-------|------|-------------|-------------|-------|
| **CPU Usage** | 📊 | 50 points | 1Hz | Dynamic (green/yellow/red) |
| **RAM Usage** | 🔹 | 50 points | 1Hz | Blue gradient |
| **Temperature** | 🌡️ | 50 points | 1Hz | Heat map (blue→red) |

### 🎮 Interactive Elements

#### **Touch Zones**
- **System Cards**: Tap any card (CPU/RAM/DISK/TEMP) for full-screen view
- **Charts**: Tap any chart area for expanded graph display
- **Coordinate Mapping**: Precise touch detection with visual feedback

#### **Full-Screen Views**
```
When touched, any element expands to full-screen:

┌─────────────────────────────────────────────────┐
│                 CPU USAGE                       │
│                                                 │
│     ████████████████████████████████████        │
│         45.2%                                   │
│                                                 │
│  ||||||||||||||||||||||||||||||||||||||||||||  │
│  Real-time scrolling graph (50 data points)    │
│                                                 │
│              Tap to return                      │
└─────────────────────────────────────────────────┘
```

### 🎨 Visual Design Features

#### **Color Schemes**
```cpp
Background:     Dark blue (#0841)
Cards:          Charcoal (#1082)
Success:        Green (#07E0)   - Normal operation
Warning:        Yellow (#FFE0)  - Attention needed
Danger:         Red (#F800)     - Critical levels
Info:           Light blue (#867F)
Text:           White (#FFFF)
Muted:          Gray (#8410)
```

#### **Typography & Icons**
- **Font**: Clean, readable system font
- **Icons**: Emoji-based indicators (📊🔹🌡️) for chart identification
- **Size**: Optimized for 2.8" displays with clear visibility
- **Contrast**: High contrast ratios for outdoor visibility

#### **Animation & Effects**
- **Smooth Scrolling**: Task Manager-style graph animation
- **Fade Transitions**: Gentle color changes between states
- **Anti-flicker**: Only updates changed regions
- **Responsive**: Immediate feedback on touch interactions

**Network Speed Display Examples:**
- Low speed: "156KB/s", "2.3KB/s"
- High speed: "5.2MB/s", "25MB/s"
- Zero activity: "0KB/s" (gray text)
- Peak values: "125MB/s" (highlighted)

## 🏗️ Architecture

### Modular Design
```
src/
├── main.cpp                 # Entry point and setup
├── DisplayManager.cpp       # Main UI coordinator (258 lines)
├── UIRenderer.cpp          # Cards, progress bars, status bar
├── GraphRenderer.cpp       # Scrolling graphs with Task Manager style
├── ScreenManager.cpp       # Startup, WiFi info, text screens
├── SystemData.cpp          # Data management and circular buffers
├── NetworkManager.cpp      # WiFi and UDP communication
└── TouchManager.cpp        # Touch input handling
```

### Key Features Implementation

**Anti-flicker System:**
- Partial screen updates only when data changes
- Smart redraw detection with threshold comparison
- Smooth visual experience

**Scrolling Graphs:**
- Windows Task Manager-style real-time graphs
- Full-width pixel distribution for smooth visualization
- Adaptive scaling with fixed ranges
- Historical data fade effects

**Modern UI:**
- Color-coded status indicators (green/yellow/red)
- Responsive card-based layout
- Professional progress bars with gradients
- Clean typography and spacing

## 🔧 Customization

### Colors
Modify colors in `include/Defines.h`:
```cpp
#define COLOR_BG 0x0841      // Background
#define COLOR_SUCCESS 0x07E0  // Green (good status)
#define COLOR_WARNING 0xFFE0  // Yellow (medium status) 
#define COLOR_DANGER 0xF800   // Red (high status)
```

### Layout
Adjust dimensions in `DisplayManager.cpp`:
```cpp
const int CARD_WIDTH = 75;
const int CARD_HEIGHT = 35;
const int GRAPH_HEIGHT = 25;
```

### Data Thresholds
Modify warning levels:
```cpp
// CPU/RAM: >60% yellow, >80% red
// Temperature: >50°C yellow, >70°C red
```

## 📈 Performance

- **Update Rate**: 1Hz for smooth real-time monitoring
- **Memory Usage**: ~48KB RAM, ~836KB Flash
- **Response Time**: <100ms for touch interactions
- **Network Latency**: <50ms for UDP data reception

## 🐛 Troubleshooting

### Display Issues
- **Blank Screen**: Check TFT pin connections and power supply
- **Wrong Colors**: Verify `TFT_RGB_ORDER` setting
- **Touch Not Working**: Confirm XPT2046 pin configuration

### Network Issues
- **No WiFi Connection**: Check credentials and signal strength
- **No Data**: Verify UDP sender is running and IP address is correct
- **Connection Drops**: Ensure stable power supply (USB can be insufficient)

### Build Issues
- **Compilation Errors**: Ensure all libraries are properly installed
- **Upload Fails**: Check COM port and ESP32 is in download mode
- **Library Conflicts**: Use exact versions specified in `platformio.ini`

## 🤝 Contributing

Contributions are welcome! Please feel free to submit pull requests or open issues for:

- New display driver support
- Additional data sources
- UI improvements
- Performance optimizations
- Documentation updates

### Development Guidelines
1. Follow the existing modular architecture
2. Maintain code documentation
3. Test on actual hardware before submitting
4. Update README for any new features

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments

- **TFT_eSPI Library** by Bodmer - Excellent ESP32 display library
- **ArduinoJson** by Benoit Blanchon - JSON parsing and generation
- **XPT2046_Touchscreen** by Paul Stoffregen - Touch controller support
- **ESP32 Community** - For extensive documentation and examples

## 📞 Support

- **Issues**: [GitHub Issues](https://github.com/asithniwantha/ESP23InfoDisplay/issues)
- **Discussions**: [GitHub Discussions](https://github.com/asithniwantha/ESP23InfoDisplay/discussions)
- **Documentation**: [Wiki](https://github.com/asithniwantha/ESP23InfoDisplay/wiki)

---

**Made with ❤️ for the ESP32 community**

*Last updated: July 2025*

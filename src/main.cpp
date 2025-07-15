/*  ESP32 System Monitor - Clean Refactored Version
    Modular architecture with separate classes for:
    - DisplayManager: All display and UI functionality
    - NetworkManager: WiFi and UDP communication
    - SystemData: System statistics management
    - TouchManager: Touchscreen handling
*/

#include <TFT_eSPI.h>
#include "Defines.h"
#include "DisplayManager.h"
#include "NetworkManager.h"
#include "SystemData.h"
#include "TouchManager.h"
#include "UIConfig.h"

// Configuration - TODO: Move to config file or environment variables
namespace Config {
    constexpr const char* WIFI_SSID = "linksys";
    constexpr const char* WIFI_PASSWORD = "asith1234567890";
}

// Component instances
DisplayManager displayManager;
NetworkManager networkManager(Config::WIFI_SSID, Config::WIFI_PASSWORD, 
                            NetworkConfig::MDNS_HOSTNAME, NetworkConfig::UDP_PORT);
SystemData systemData;
TouchManager touchManager(XPT2046_CS, XPT2046_IRQ);

void setup() {
    Serial.begin(115200);
    Serial.println("Starting ESP32 System Monitor...");

    // Initialize display
    displayManager.begin();
    displayManager.showStartupScreen();

    // Show WiFi loading screen
    //displayManager.showWiFiLoadingScreen();

    // Initialize network
    networkManager.begin();

    // Hide WiFi loading screen after connection
    //displayManager.hideWiFiLoadingScreen();
    
    // Brief WiFi info display (2 seconds)
    displayManager.displayWiFiInfo(networkManager.getLocalIP(), 
                                   NetworkConfig::MDNS_HOSTNAME, 
                                   NetworkConfig::UDP_PORT);
    delay(2000);

    // Initialize touchscreen
    touchManager.begin();
    
    // Initialize system data
    systemData.reset();
    
    // Show clock screen after all initialization is complete
    displayManager.showClockAfterSetup();
    
    Serial.println("System initialized successfully!");
    Serial.println("Showing clock until UDP data received...");
}

void loop() {
    // Handle LVGL tasks
    displayManager.tick();
    
    // Handle incoming UDP packets
    String packet = networkManager.getReceivedPacket();
    if (packet.length() > 0) {
        Serial.printf("Received packet: %s\n", packet.c_str());
        Serial.printf("From: %s, port %d\n", networkManager.getRemoteIP().c_str(), networkManager.getRemotePort());

        if (packet.startsWith("{")) {
            // JSON system data - update and display modern UI
            systemData.update(packet);
            displayManager.displayModernUI(systemData);
        } else {
            // Plain text message - display as text
            displayManager.displayUDPText(packet);
            systemData.reset(); // Reset for next JSON display
        }
    }

    // Handle touchscreen input
    if (touchManager.isTouched()) {
        TS_Point p = touchManager.getPoint();
        touchManager.calibrate(p);
        
        touchManager.printToSerial(p.x, p.y, p.z);
        
        // Check if touch is in volume control area
        if (p.x >= TouchZones::VOLUME_CONTROL_X_MIN && p.x <= TouchZones::VOLUME_CONTROL_X_MAX &&
            p.y >= TouchZones::VOLUME_CONTROL_Y_MIN && p.y <= TouchZones::VOLUME_CONTROL_Y_MAX) {
            displayManager.handleVolumeTouch(p.x, p.y, systemData);
        } else {
            displayManager.handleElementTouch(p.x, p.y);
        }

        delay(100); // Debounce
    }
    
    // Check for data timeout and switch to clock if needed
    displayManager.checkDataTimeout();
    
    // Update clock display if no recent data
    displayManager.updateClock(&networkManager);

    delay(10); // Small delay to prevent CPU overload
}
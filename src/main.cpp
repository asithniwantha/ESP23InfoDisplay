/*  ESP32 System Monitor - Clean Refactored Version
    Modular architecture with separate classes for:
    - DisplayManager: All display and UI functionality
    - NetworkManager: WiFi and UDP communication
    - SystemData: System statistics management
    - TouchManager: Touchscreen handling
*/

#include "Defines.h"
#include "DisplayManager.h"
#include "NetworkManager.h"
#include "SystemData.h"
#include "TouchManager.h"

// Configuration
const char* ssid = "linksys";
const char* password = "asith1234567890";
const char* mdnsHostname = "esp32display";
const int udpPort = 8080;

// Component instances
DisplayManager displayManager;
NetworkManager networkManager(ssid, password, mdnsHostname, udpPort);
SystemData systemData;
TouchManager touchManager(XPT2046_CS, XPT2046_IRQ);

void setup() {
    Serial.begin(115200);
    Serial.println("Starting ESP32 System Monitor...");

    // Initialize display
    displayManager.begin();
    displayManager.showStartupScreen();

    // Initialize network
    networkManager.begin();
    displayManager.displayWiFiInfo(networkManager.getLocalIP(), mdnsHostname, udpPort);

    // Initialize touchscreen
    touchManager.begin();
    
    // Initialize system data
    systemData.reset();
    
    Serial.println("System initialized successfully!");
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
        displayManager.printTouchToDisplay(p.x, p.y, p.z);
        systemData.reset(); // Reset for next JSON display

        delay(100); // Debounce
    }

    delay(10); // Small delay to prevent CPU overload
}

#include <TFT_eSPI.h>
#include "lv_font_digital_80.h"
#include "Defines.h"
#include "ui/UIManager.h"
#include "NetworkManager.h"
#include "SystemData.h"
#include "UIConfig.h"

namespace Config {
    constexpr const char* WIFI_SSID = "linksys";
    constexpr const char* WIFI_PASSWORD = "asith1234567890";
}

SystemData systemData;
NetworkManager networkManager(Config::WIFI_SSID, Config::WIFI_PASSWORD, 
                            NetworkConfig::MDNS_HOSTNAME, NetworkConfig::UDP_PORT);
UIManager uiManager(systemData, networkManager);

void setup() {
    Serial.begin(115200);
    Serial.println("Starting ESP32 System Monitor...");

    // Simple TFT_eSPI startup screen
    static TFT_eSPI tft = TFT_eSPI();
    tft.init();
    tft.setRotation(1); // Adjust rotation as needed
    tft.invertDisplay( true );
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextDatum(MC_DATUM);
    tft.setFreeFont(&FreeSansBold12pt7b); // Use built-in font or comment out if not available
    tft.drawString("ESP32 System Monitor", tft.width()/2, tft.height()/2);
    tft.drawString("Connecting to wifi ..", tft.width()/2, tft.height()/2 + 30);
    Serial.println("TFT initialized");
    Serial.println("[DEBUG] Calling uiManager.init()");
    uiManager.init();
    Serial.println("[DEBUG] uiManager.init() done");

    Serial.println("[DEBUG] Showing startup screen");
    uiManager.showStartupScreen();
    Serial.println("[DEBUG] Startup screen shown");

    Serial.println("[DEBUG] Calling networkManager.begin()");
    networkManager.begin();
    Serial.println("[DEBUG] networkManager.begin() done");

    Serial.println("[DEBUG] Showing WiFi info");
    uiManager.showWiFiInfo(networkManager.getLocalIP(), 
                                   NetworkConfig::MDNS_HOSTNAME, 
                                   NetworkConfig::UDP_PORT);
    delay(2000);

    Serial.println("[DEBUG] Resetting systemData");
    systemData.reset();
    // Wait for NTP time sync before showing clock screen
    Serial.println("[DEBUG] Waiting for NTP time sync...");
    time_t now = 0;
    int waitCount = 0;
    while ((now = time(nullptr)) < 1609459200 && waitCount < 100) { // 1609459200 = Jan 1, 2021
        delay(100);
        waitCount++;
    }
    struct tm* t = localtime(&now);
    Serial.printf("[DEBUG] System time after NTP: %04d-%02d-%02d %02d:%02d:%02d\n", t->tm_year+1900, t->tm_mon+1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
    Serial.println("[DEBUG] Showing clock screen");
    uiManager.showClockScreen();
    Serial.println("System initialized successfully!");
}

void loop() {
    uiManager.tick();
    String packet = networkManager.getReceivedPacket();
    Serial.print("[DEBUG] Packet: "); Serial.println(packet);
    if (packet.length() > 0) {
        uiManager.setDataReceived(); // Mark data as received
        if (packet.startsWith("{")) {
            Serial.println("[DEBUG] JSON packet");
            systemData.update(packet);
            uiManager.update();
        } else {
            Serial.println("[DEBUG] Text packet");
            uiManager.showUDPText(packet);
            systemData.reset();
        }
    }
    delay(10); // Small delay to prevent CPU overload
}

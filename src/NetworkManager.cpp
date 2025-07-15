#include "NetworkManager.h"

NetworkManager::NetworkManager(const char* ssid, const char* password, const char* mdnsHostname, int udpPort)
    : ssid(ssid), password(password), mdnsHostname(mdnsHostname), udpPort(udpPort), timesSynced(false) {}

void NetworkManager::begin() {
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi");
    
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    
    Serial.println();
    Serial.println("WiFi connected!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    
    // Synchronize time with NTP servers
    syncTime();
    
    if (MDNS.begin(mdnsHostname)) {
        Serial.printf("mDNS responder started: %s.local\n", mdnsHostname);
        MDNS.addService("udp", "udp", udpPort);
    } else {
        Serial.println("Error starting mDNS");
    }
    
    udp.begin(udpPort);
    Serial.printf("UDP Listening on IP: %s, UDP port: %d\n", WiFi.localIP().toString().c_str(), udpPort);
}

String NetworkManager::getReceivedPacket() {
    int packetSize = udp.parsePacket();
    if (packetSize) {
        int len = udp.read(incomingPacket, 255);
        if (len > 0) {
            incomingPacket[len] = 0;
        }
        return String(incomingPacket);
    }
    return "";
}

String NetworkManager::getRemoteIP() {
    return udp.remoteIP().toString();
}

int NetworkManager::getRemotePort() {
    return udp.remotePort();
}

String NetworkManager::getLocalIP() {
    return WiFi.localIP().toString();
}

void NetworkManager::syncTime() {
    Serial.println("Synchronizing time with NTP servers...");
    
    // Configure time with NTP servers (Sri Lanka UTC+5:30, no daylight saving)
    // 5.5 hours = 19800 seconds offset
    configTime(19800, 0, "pool.ntp.org", "time.nist.gov", "time.google.com");
    
    // Wait for time to be synchronized
    struct tm timeinfo;
    int attempts = 0;
    while (!getLocalTime(&timeinfo) && attempts < 10) {
        Serial.print(".");
        delay(1000);
        attempts++;
    }
    
    if (attempts < 10) {
        timesSynced = true;
        Serial.println();
        Serial.println("Time synchronized successfully!");
        Serial.printf("Current time: %s", asctime(&timeinfo));
    } else {
        timesSynced = false;
        Serial.println();
        Serial.println("Failed to synchronize time");
    }
}

bool NetworkManager::isTimeSync() {
    return timesSynced;
}

String NetworkManager::getCurrentTime() {
    if (!timesSynced) return "00:00:00";
    
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        return "00:00:00";
    }
    
    char timeStr[9];
    strftime(timeStr, sizeof(timeStr), "%H:%M:%S", &timeinfo);
    return String(timeStr);
}

String NetworkManager::getCurrentDate() {
    if (!timesSynced) return "Date not available";
    
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        return "Date not available";
    }
    
    char dateStr[32];
    strftime(dateStr, sizeof(dateStr), "%A, %B %d, %Y", &timeinfo);
    return String(dateStr);
}

String NetworkManager::getCurrentTime12Hour() {
    if (!timesSynced) return "12:00:00";
    
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        return "12:00:00";
    }
    
    char timeStr[12];
    strftime(timeStr, sizeof(timeStr), "%I:%M:%S", &timeinfo);
    return String(timeStr);
}

String NetworkManager::getAMPM() {
    if (!timesSynced) return "AM";
    
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        return "AM";
    }
    
    char ampmStr[3];
    strftime(ampmStr, sizeof(ampmStr), "%p", &timeinfo);
    return String(ampmStr);
}

#include "NetworkManager.h"

NetworkManager::NetworkManager(const char* ssid, const char* password, const char* mdnsHostname, int udpPort)
    : ssid(ssid), password(password), mdnsHostname(mdnsHostname), udpPort(udpPort) {}

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

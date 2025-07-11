#ifndef NETWORK_MANAGER_H
#define NETWORK_MANAGER_H

#include <WiFi.h>
#include <WiFiUdp.h>
#include <ESPmDNS.h>

class NetworkManager {
public:
    NetworkManager(const char* ssid, const char* password, const char* mdnsHostname, int udpPort);
    void begin();
    String getReceivedPacket();
    String getRemoteIP();
    int getRemotePort();
    String getLocalIP();

private:
    const char* ssid;
    const char* password;
    const char* mdnsHostname;
    int udpPort;
    WiFiUDP udp;
    char incomingPacket[255];
};

#endif // NETWORK_MANAGER_H

#ifndef NETWORK_MANAGER_H
#define NETWORK_MANAGER_H

#include <WiFi.h>
#include <WiFiUdp.h>
#include <ESPmDNS.h>
#include <time.h>

class NetworkManager {
public:
    NetworkManager(const char* ssid, const char* password, const char* mdnsHostname, int udpPort);
    void begin();
    String getReceivedPacket();
    String getRemoteIP();
    int getRemotePort();
    String getLocalIP();
    void syncTime();
    bool isTimeSync();
    String getCurrentTime();
    String getCurrentDate();
    String getCurrentTime12Hour(); // 12-hour format with AM/PM
    String getAMPM(); // Get AM/PM separately for styling

private:
    const char* ssid;
    const char* password;
    const char* mdnsHostname;
    int udpPort;
    WiFiUDP udp;
    char incomingPacket[255];
    bool timesSynced;
};

#endif // NETWORK_MANAGER_H

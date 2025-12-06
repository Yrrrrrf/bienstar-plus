#ifndef NETWORK_MANAGER_H
#define NETWORK_MANAGER_H

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "time.h"

class NetworkManager {
public:
    NetworkManager(const char* ssid, const char* password, const char* serverUrl);
    
    void begin();
    void update(); // Keep connection alive
    bool sendTelemetry(int hr, int spo2, float temp, const char* status);
    String getFormattedTime();
    bool isConnected();

private:
    const char* _ssid;
    const char* _password;
    const char* _serverUrl;
    
    // NTP Settings
    const char* _ntpServer = "pool.ntp.org";
    const long _gmtOffset_sec = -21600; // Mexico Central
    const int _daylightOffset_sec = 0;
};

#endif

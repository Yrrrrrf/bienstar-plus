#include "NetworkManager.h"

NetworkManager::NetworkManager(const char* ssid, const char* password, const char* serverUrl) {
    _ssid = ssid;
    _password = password;
    _serverUrl = serverUrl;
}

void NetworkManager::begin() {
    WiFi.begin(_ssid, _password);
    int retries = 0;
    while (WiFi.status() != WL_CONNECTED && retries < 10) {
        delay(500);
        Serial.print(".");
        retries++;
    }
    
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nWiFi Connected!");
        configTime(_gmtOffset_sec, _daylightOffset_sec, _ntpServer);
    }
}

bool NetworkManager::sendTelemetry(int hr, int spo2, float temp, const char* status) {
    if (WiFi.status() != WL_CONNECTED) return false;

    HTTPClient http;
    http.begin(_serverUrl);
    http.addHeader("Content-Type", "application/json");

    // Construct JSON Payload
    String jsonPayload = "{";
    jsonPayload += "\"hr\":" + String(hr) + ",";
    jsonPayload += "\"spo2\":" + String(spo2) + ",";
    jsonPayload += "\"temp\":" + String(temp, 1) + ",";
    jsonPayload += "\"status\":\"" + String(status) + "\"";
    jsonPayload += "}";

    int httpResponseCode = http.POST(jsonPayload);
    http.end();

    return (httpResponseCode > 0);
}

String NetworkManager::getFormattedTime() {
    struct tm timeinfo;
    if(!getLocalTime(&timeinfo)){
        return "--:--";
    }
    char timeString[6];
    strftime(timeString, 6, "%H:%M", &timeinfo);
    return String(timeString);
}

bool NetworkManager::isConnected() {
    return WiFi.status() == WL_CONNECTED;
}

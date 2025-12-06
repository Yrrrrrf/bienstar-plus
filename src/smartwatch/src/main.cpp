#include <Arduino.h>
#include "NetworkManager.h"
#include "SensorManager.h"
#include "UIManager.h"

// --- Configuration ---
const char* WIFI_SSID = "YOUR_WIFI_NAME";
const char* WIFI_PASS = "YOUR_WIFI_PASS";
const char* SERVER_URL = "http://192.168.1.50:5173/api/vitals"; // Change IP to your PC
const int BUZZER_PIN = 18;

// --- Objects ---
NetworkManager network(WIFI_SSID, WIFI_PASS, SERVER_URL);
SensorManager sensors;
UIManager ui(BUZZER_PIN);

// --- Timers ---
unsigned long lastScreenUpdate = 0;
unsigned long lastTelemetrySend = 0;
const int SCREEN_INTERVAL = 200;    // 5 FPS
const int TELEMETRY_INTERVAL = 2000; // Send data every 2s

void setup() {
    Serial.begin(115200);
    
    // Initialize Modules
    ui.begin();
    sensors.begin();
    network.begin();
}

void loop() {
    // 1. Critical Loop: Pulse detection must run fast
    sensors.update();

    // 2. Get current data snapshot
    VitalsData currentData = sensors.getReadings();
    
    // 3. UI & Alerts Logic
    ui.checkAlerts(currentData);
    
    if (millis() - lastScreenUpdate > SCREEN_INTERVAL) {
        lastScreenUpdate = millis();
        ui.updateScreen(
            currentData, 
            network.getFormattedTime(), 
            network.isConnected()
        );
    }

    // 4. Telemetry (HTTP Post)
    if (millis() - lastTelemetrySend > TELEMETRY_INTERVAL) {
        lastTelemetrySend = millis();
        
        // Determine status string
        const char* status = "normal";
        if (currentData.bpm > 120 || currentData.temperature > 37.5) {
            status = "critical";
        } else if (currentData.bpm < 50 && currentData.validFinger) {
            status = "warning";
        }

        // Only send if we have valid readings or to report connectivity
        if (currentData.validFinger || currentData.temperature > 0) {
            network.sendTelemetry(
                currentData.bpm, 
                currentData.spo2, 
                currentData.temperature, 
                status
            );
            Serial.println("Telemetry Sent.");
        }
    }
}

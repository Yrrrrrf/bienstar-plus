#include "UIManager.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

UIManager::UIManager(int buzzerPin) 
    : _display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1) {
    _buzzerPin = buzzerPin;
    _alarmActive = false;
    _lastBuzzerToggle = 0;
    _buzzerState = false;
}

void UIManager::begin() {
    pinMode(_buzzerPin, OUTPUT);
    digitalWrite(_buzzerPin, LOW);

    if(!_display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println("Display Allocation Failed");
        for(;;);
    }
    _display.clearDisplay();
    _display.setTextColor(WHITE);
    _display.setTextSize(1);
    _display.setCursor(0,20);
    _display.println("Bienestar+ Init...");
    _display.display();
}

void UIManager::checkAlerts(VitalsData vitals) {
    // Determine if critical
    bool tempAlert = (vitals.temperature > MAX_TEMP);
    bool hrAlert = (vitals.validFinger) && (vitals.bpm > MAX_BPM || vitals.bpm < MIN_BPM);

    _alarmActive = tempAlert || hrAlert;

    if (_alarmActive) {
        // Non-blocking buzzer tone
        if (millis() - _lastBuzzerToggle > 200) {
            _lastBuzzerToggle = millis();
            _buzzerState = !_buzzerState;
            digitalWrite(_buzzerPin, _buzzerState);
        }
    } else {
        digitalWrite(_buzzerPin, LOW);
    }
}

void UIManager::updateScreen(VitalsData vitals, String timeStr, bool wifiConnected) {
    _display.clearDisplay();

    // Top Bar
    _display.setTextSize(1);
    _display.setCursor(0, 0);
    _display.print(wifiConnected ? "WIFI OK" : "OFFLINE");
    _display.setCursor(90, 0);
    _display.print(timeStr);
    _display.drawLine(0, 10, 128, 10, WHITE);

    // Vitals
    if (!vitals.validFinger) {
        _display.setCursor(30, 30);
        _display.print("PLACE FINGER");
    } else {
        // Heart Rate
        _display.setTextSize(2);
        _display.setCursor(0, 20);
        _display.print(vitals.bpm);
        _display.setTextSize(1);
        _display.print(" bpm");

        // Temp
        _display.setCursor(0, 45);
        _display.print("T: ");
        _display.print(vitals.temperature, 1);
        _display.print(" C");

        // SpO2
        _display.setCursor(70, 45);
        _display.print("O2: ");
        _display.print(vitals.spo2);
        _display.print("%");
    }

    // Alarm Status
    if (_alarmActive) {
        _display.setCursor(80, 20);
        _display.print("ALERT!");
    }

    _display.display();
}

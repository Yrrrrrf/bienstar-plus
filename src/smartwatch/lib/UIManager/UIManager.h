#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "../SensorManager/SensorManager.h" // To know about VitalsData

class UIManager {
public:
    UIManager(int buzzerPin);
    void begin();
    void updateScreen(VitalsData vitals, String timeStr, bool wifiConnected);
    void checkAlerts(VitalsData vitals);

private:
    int _buzzerPin;
    Adafruit_SSD1306 _display;
    
    // Alert State
    bool _alarmActive;
    long _lastBuzzerToggle;
    bool _buzzerState;
    
    // Constants
    const float MAX_TEMP = 37.5;
    const int MAX_BPM = 120;
    const int MIN_BPM = 50;
};

#endif

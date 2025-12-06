#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>
#include "MAX30105.h"
#include "heartRate.h"

struct VitalsData {
    int bpm;
    int spo2;
    float temperature;
    bool validFinger; // Is finger placed?
};

class SensorManager {
public:
    SensorManager();
    void begin();
    void update(); // Must be called very frequently (in loop)
    VitalsData getReadings();

private:
    MAX30105 _particleSensor;
    Adafruit_BMP280 _bmp;
    
    bool _bmpAvailable;
    
    // Heart Rate calculation vars
    const byte RATE_SIZE = 4;
    byte _rates[4];
    byte _rateSpot;
    long _lastBeat;
    float _beatsPerMinute;
    int _beatAvg;
    
    long _lastUpdate;
};

#endif

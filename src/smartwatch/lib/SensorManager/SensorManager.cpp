#include "SensorManager.h"

SensorManager::SensorManager() {
    _bmpAvailable = false;
    _rateSpot = 0;
    _lastBeat = 0;
    _beatAvg = 0;
}

void SensorManager::begin() {
    // 1. BMP280 Setup
    if (_bmp.begin(0x76)) {
        _bmpAvailable = true;
    } else if (_bmp.begin(0x77)) {
        _bmpAvailable = true;
    }

    // 2. MAX30105 Setup
    if (_particleSensor.begin(Wire, I2C_SPEED_FAST)) {
        _particleSensor.setup(); 
        _particleSensor.setPulseAmplitudeRed(0x0A); // Low power to avoid noise
        _particleSensor.setPulseAmplitudeGreen(0);
    }
}

void SensorManager::update() {
    long irValue = _particleSensor.getIR();

    // Finger detection threshold
    if (irValue < 50000) {
        _beatAvg = 0;
        return;
    }

    if (checkForBeat(irValue) == true) {
        long delta = millis() - _lastBeat;
        _lastBeat = millis();

        _beatsPerMinute = 60 / (delta / 1000.0);

        if (_beatsPerMinute < 255 && _beatsPerMinute > 20) {
            _rates[_rateSpot++] = (byte)_beatsPerMinute;
            _rateSpot %= RATE_SIZE;

            _beatAvg = 0;
            for (byte x = 0 ; x < RATE_SIZE ; x++) _beatAvg += _rates[x];
            _beatAvg /= RATE_SIZE;
        }
    }
}

VitalsData SensorManager::getReadings() {
    VitalsData data;
    data.bpm = _beatAvg;
    
    // Mock SpO2 Calculation logic (Real math requires raw RED/IR buffers)
    // For this prototype, if pulse is good, we estimate based on typical healthy ranges
    // randomized slightly for realism.
    if (_beatAvg > 0) {
        data.spo2 = 96 + (rand() % 4); // 96-99%
        data.validFinger = true;
    } else {
        data.spo2 = 0;
        data.validFinger = false;
    }

    if (_bmpAvailable) {
        data.temperature = _bmp.readTemperature();
    } else {
        data.temperature = 0.0;
    }

    return data;
}

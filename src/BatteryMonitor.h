#pragma once
#include <Arduino.h>

class BatteryMonitor {
private:
    uint8_t m_pin;
    float m_r1;
    float m_r2;
    float m_vcc;
    
    unsigned long m_lastReadTime;
    unsigned long m_interval;
    int m_latestTelemetryValue; // Stores voltage as integer (e.g., 1240 for 12.40V)

public:
    // Pass pin, measured resistors (ohms), VCC, and read interval (ms)
    BatteryMonitor(uint8_t pin, float r1, float r2, float vcc = 5.00, unsigned long intervalMs = 500)
        : m_pin(pin), m_r1(r1), m_r2(r2), m_vcc(vcc), m_lastReadTime(0), m_interval(intervalMs), m_latestTelemetryValue(1200) {}

    void begin() {
        pinMode(m_pin, INPUT);
    }

    // Call this inside your main loop to handle non-blocking background updates
    void update() {
        unsigned long currentTime = millis();
        if (currentTime - m_lastReadTime >= m_interval) {
            m_lastReadTime = currentTime;

            int rawADC = analogRead(m_pin);
            float vPin = (rawADC * m_vcc) / 1023.0; 
            float trueVoltage = vPin * ((m_r1 + m_r2) / m_r2);

            // Convert to your framework's required integer format (e.g. 12.43V -> 1243)
            m_latestTelemetryValue = (int)(trueVoltage * 100.0);
        }
    }

    // Returns the cached telemetry value instantly without stopping to read the ADC
    int getTelemetryVoltage() const {
        return m_latestTelemetryValue;
    }
};

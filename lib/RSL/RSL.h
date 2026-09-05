#pragma once

#include <Arduino.h>

class RSL
{
public:
    explicit RSL(uint8_t pin, bool inverted = false);

    void begin();

    void setEnabled(bool enabled);

    void update();

private:
    uint8_t m_pin;
    bool m_inverted;
    bool m_enabled;

    bool m_outputState;
    unsigned long m_lastToggle;

    void setOutput(bool on);
};
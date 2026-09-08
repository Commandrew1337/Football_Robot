#pragma once

#include <Arduino.h>

// Robot Signal Light (RSL)
//
// Enabled  -> Fast blink
// Disabled -> Slow blink
//
// The output is initialized automatically by the constructor.
// No begin() call is required.

class RSL
{
public:
    // Create an RSL object.
    //
    // pin      = Output pin driving the light or MOSFET.
    // inverted = True if the hardware is active-low.
    RSL(uint8_t pin, bool inverted = false);

    // Set robot enabled state.
    void setEnabled(bool enabled);

    // Update blink timing.
    // Call continuously from loop().
    void update();

private:
    void setOutput(bool on);

    uint8_t m_pin;
    bool m_inverted;
    bool m_enabled;
    bool m_outputState;
    unsigned long m_lastToggle;
};
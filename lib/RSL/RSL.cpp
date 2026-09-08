#include "RSL.h"

namespace
{
    constexpr unsigned long ENABLED_BLINK_MS = 125;   // Fast blink
    constexpr unsigned long DISABLED_BLINK_MS = 500;  // Slow blink
}

// Constructor.
RSL::RSL(uint8_t pin, bool inverted)
    : m_pin(pin),
      m_inverted(inverted),
      m_enabled(false),
      m_outputState(false),
      m_lastToggle(0)
{
    // Configure output pin.
    pinMode(m_pin, OUTPUT);

    // Ensure light starts OFF.
    setOutput(false);
}

// Set whether the robot is enabled.
void RSL::setEnabled(bool enabled)
{
    m_enabled = enabled;
}

// Update blink state.
void RSL::update()
{
    const unsigned long now = millis();

    const unsigned long blinkRate =
        m_enabled ? ENABLED_BLINK_MS : DISABLED_BLINK_MS;

    if (now - m_lastToggle >= blinkRate)
    {
        m_outputState = !m_outputState;
        setOutput(m_outputState);
        m_lastToggle = now;
    }
}

// Apply output state to hardware.
void RSL::setOutput(bool on)
{
    digitalWrite(
        m_pin,
        (on ^ m_inverted) ? HIGH : LOW);
}
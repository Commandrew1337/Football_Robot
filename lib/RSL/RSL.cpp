#include "RSL.h"

namespace
{
    constexpr unsigned long ENABLED_BLINK_MS = 125;   // Fast blink
    constexpr unsigned long DISABLED_BLINK_MS = 500;  // Slow blink
}

RSL::RSL(uint8_t pin, bool inverted)
    : m_pin(pin),
      m_inverted(inverted),
      m_enabled(false),
      m_outputState(false),
      m_lastToggle(0)
{
}

void RSL::begin()
{
    pinMode(m_pin, OUTPUT);
    setOutput(false);
}

void RSL::setEnabled(bool enabled)
{
    m_enabled = enabled;
}

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

void RSL::setOutput(bool on)
{
    digitalWrite(
        m_pin,
        (on ^ m_inverted) ? HIGH : LOW);
}

/*
#include "RSL.h"

RSL rsl(8); // MOSFET input on D8

void setup()
{
    rsl.begin();
}

void loop()
{
    bool robotEnabled = true; // Replace with your enable logic

    rsl.setEnabled(robotEnabled);

    rsl.update();
}
*/
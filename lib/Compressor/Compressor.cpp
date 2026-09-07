#include "Compressor.h"

Compressor::Compressor(
    uint8_t pressureSwitchPin,
    uint8_t spikeForwardPin,
    uint8_t spikeReversePin)
    : _pressureSwitchPin(pressureSwitchPin),
      _spike(spikeForwardPin, spikeReversePin),
      _enabled(true),
      _running(false)
{
    // FRC pressure switches normally close when
    // system pressure is below cutoff pressure.
    //
    // Using INPUT_PULLUP allows:
    //
    // Open switch   = HIGH
    // Closed switch = LOW
    //
    pinMode(_pressureSwitchPin, INPUT_PULLUP);

    _spike.off();
}

void Compressor::update()
{
    if (!_enabled)
    {
        _spike.off();
        _running = false;
        return;
    }

    // Closed = low pressure
    // Open   = pressure reached

    if (digitalRead(_pressureSwitchPin) == LOW)
    {
        _spike.forward();
        _running = true;
    }
    else
    {
        _spike.off();
        _running = false;
    }
}

void Compressor::stop()
{
    _spike.off();
    _running = false;
}

void Compressor::enable()
{
    _enabled = true;
}

void Compressor::disable()
{
    _enabled = false;

    _spike.off();
    _running = false;
}

bool Compressor::isPressurized() const
{
    return digitalRead(_pressureSwitchPin) == HIGH;
}

bool Compressor::isRunning() const
{
    return _running;
}

bool Compressor::isEnabled() const
{
    return _enabled;
}

/*
#include "Compressor.h"

// Pressure switch on D30
// SPIKE Forward on D22
// SPIKE Reverse on D23

Compressor compressor(30, 22, 23);

void setup()
{
}

void loop()
{
    compressor.update();
}
*/
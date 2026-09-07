#pragma once

#include <Arduino.h>
#include "SpikeRelay.h"

// Controls an FRC compressor using:
//
// - One SPIKE relay
// - One FRC pressure switch
//
// Typical wiring:
//
// Pressure Switch
//   One side -> Arduino GND
//   Other side -> Arduino input pin
//
// Compressor SPIKE
//   White -> Forward pin
//   Red   -> Reverse pin
//   Black -> Arduino GND
//
class Compressor
{
public:
    // pressureSwitchPin
    //      Arduino input connected to the pressure switch.
    //
    // spikeForwardPin
    //      SPIKE white wire.
    //
    // spikeReversePin
    //      SPIKE red wire.
    Compressor(
        uint8_t pressureSwitchPin,
        uint8_t spikeForwardPin,
        uint8_t spikeReversePin);

    // Call periodically from loop().
    void update();

    // Force compressor off.
    void stop();

    // Enable automatic pressure regulation.
    void enable();

    // Disable compressor and stop motor.
    void disable();

    // Returns true if pressure switch indicates system is full.
    bool isPressurized() const;

    // Returns true if compressor is currently running.
    bool isRunning() const;

    // Returns true if automatic mode is enabled.
    bool isEnabled() const;

private:
    uint8_t _pressureSwitchPin;

    SpikeRelay _spike;

    bool _enabled;
    bool _running;
};
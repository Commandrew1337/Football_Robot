#pragma once

#include <Arduino.h>

// Controls an FRC/VEX SPIKE relay module.
//
// White wire = Forward input
// Red wire   = Reverse input
// Black wire = Ground
//
// States:
//   forward() = Forward output
//   reverse() = Reverse output
//   off()     = Off / Brake

class SpikeRelay
{
public:
    // Create a SPIKE relay object.
    //
    // forwardPin -> White wire
    // reversePin -> Red wire
    //
    // Pins are automatically configured as outputs and
    // initialized to the OFF state.
    SpikeRelay(uint8_t forwardPin, uint8_t reversePin);

    // Command forward.
    void forward();

    // Command reverse.
    void reverse();

    // Turn relay off.
    void off();

    // State queries.
    bool isForward() const;
    bool isReverse() const;
    bool isOff() const;

private:
    enum class State
    {
        Off,
        Forward,
        Reverse
    };

    uint8_t _forwardPin;
    uint8_t _reversePin;
    State _state;
};
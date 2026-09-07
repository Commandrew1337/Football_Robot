#include "SpikeRelay.h"

// Constructor.
SpikeRelay::SpikeRelay(uint8_t forwardPin, uint8_t reversePin)
    : _forwardPin(forwardPin),
      _reversePin(reversePin),
      _state(State::Off)
{
    // Configure Arduino pins.
    pinMode(_forwardPin, OUTPUT);
    pinMode(_reversePin, OUTPUT);

    // Ensure the relay starts OFF.
    off();
}

// Drive relay forward.
//
// White = HIGH
// Red   = LOW
void SpikeRelay::forward()
{
    digitalWrite(_forwardPin, HIGH);
    digitalWrite(_reversePin, LOW);

    _state = State::Forward;
}

// Drive relay reverse.
//
// White = LOW
// Red   = HIGH
void SpikeRelay::reverse()
{
    digitalWrite(_forwardPin, LOW);
    digitalWrite(_reversePin, HIGH);

    _state = State::Reverse;
}

// Turn relay off.
//
// White = LOW
// Red   = LOW
void SpikeRelay::off()
{
    digitalWrite(_forwardPin, LOW);
    digitalWrite(_reversePin, LOW);

    _state = State::Off;
}

// Returns true if relay is commanded forward.
bool SpikeRelay::isForward() const
{
    return _state == State::Forward;
}

// Returns true if relay is commanded reverse.
bool SpikeRelay::isReverse() const
{
    return _state == State::Reverse;
}

// Returns true if relay is commanded off.
bool SpikeRelay::isOff() const
{
    return _state == State::Off;
}

/*
#include "SpikeRelay.h"

// Compressor controlled by a SPIKE.
SpikeRelay compressorSpike(22, 23);

// Roller motor controlled by a SPIKE.
SpikeRelay rollerSpike(24, 25);

void setup()
{
}

void loop()
{
    rollerSpike.forward();
    delay(2000);

    rollerSpike.reverse();
    delay(2000);

    rollerSpike.off();
    delay(1000);
}
*/
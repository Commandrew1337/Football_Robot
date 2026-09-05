#include "RelayValve.h"

RelayValve::RelayValve(
    uint8_t relayPin,
    bool activeLow)
    : _type(ValveType::SingleSolenoid),
      _relayA(relayPin),
      _relayB(255),
      _activeLow(activeLow),
      _pulseTimeMs(100)
{
    pinMode(_relayA, OUTPUT);

    // Start de-energized.
    relayOff(_relayA);
}

RelayValve::RelayValve(
    uint8_t relayAPin,
    uint8_t relayBPin,
    uint16_t pulseTimeMs,
    bool activeLow)
    : _type(ValveType::DoubleSolenoid),
      _relayA(relayAPin),
      _relayB(relayBPin),
      _activeLow(activeLow),
      _pulseTimeMs(pulseTimeMs)
{
    pinMode(_relayA, OUTPUT);
    pinMode(_relayB, OUTPUT);

    // Start with both coils de-energized.
    relayOff(_relayA);
    relayOff(_relayB);
}

void RelayValve::relayOn(uint8_t pin)
{
    digitalWrite(
        pin,
        _activeLow ? LOW : HIGH);
}

void RelayValve::relayOff(uint8_t pin)
{
    digitalWrite(
        pin,
        _activeLow ? HIGH : LOW);
}

void RelayValve::activate()
{
    if (_type != ValveType::SingleSolenoid)
    {
        return;
    }

    relayOn(_relayA);
}

void RelayValve::deactivate()
{
    if (_type != ValveType::SingleSolenoid)
    {
        return;
    }

    relayOff(_relayA);
}

void RelayValve::setPositionA()
{
    if (_type != ValveType::DoubleSolenoid)
    {
        return;
    }

    // Ensure opposite coil is off.
    relayOff(_relayB);

    // Pulse coil A.
    relayOn(_relayA);
    delay(_pulseTimeMs);
    relayOff(_relayA);
}

void RelayValve::setPositionB()
{
    if (_type != ValveType::DoubleSolenoid)
    {
        return;
    }

    // Ensure opposite coil is off.
    relayOff(_relayA);

    // Pulse coil B.
    relayOn(_relayB);
    delay(_pulseTimeMs);
    relayOff(_relayB);
}

void RelayValve::setPulseTime(
    uint16_t pulseTimeMs)
{
    _pulseTimeMs = pulseTimeMs;
}
    /*
    #include <Arduino.h>
#include "RelayValve.h"

// Single-solenoid spring-return valves
RelayValve singleSolenoidValveA(22);
RelayValve singleSolenoidValveB(23);

// Double-solenoid bistable valve
RelayValve doubleSolenoidValve(
    24,     // Coil A relay output
    25,     // Coil B relay output
    100);   // Pulse duration (ms)

void setup()
{
}

void loop()
{
    singleSolenoidValveA.activate();
    delay(1000);

    singleSolenoidValveA.deactivate();
    delay(1000);

    singleSolenoidValveB.activate();
    delay(1000);

    singleSolenoidValveB.deactivate();
    delay(1000);

    doubleSolenoidValve.setPositionA();
    delay(2000);

    doubleSolenoidValve.setPositionB();
    delay(2000);
}
    */
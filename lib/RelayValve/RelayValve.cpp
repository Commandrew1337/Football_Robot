#include "RelayValve.h"

// SINGLE SOLENOID CONSTRUCTOR
RelayValve::RelayValve(uint8_t relayPin, bool activeLow)
    : _type(ValveType::SingleSolenoid), _relayA(relayPin), _relayB(255), 
      _activeLow(activeLow), _pulseTimeMs(100), _currentState(false) { // <-- FIXED: Added state tracking initialization
    pinMode(_relayA, OUTPUT);
    relayOff(_relayA);
}

// DOUBLE SOLENOID CONSTRUCTOR
RelayValve::RelayValve(uint8_t relayAPin, uint8_t relayBPin, uint16_t pulseTimeMs, bool activeLow)
    : _type(ValveType::DoubleSolenoid), _relayA(relayAPin), _relayB(relayBPin), 
      _activeLow(activeLow), _pulseTimeMs(pulseTimeMs), _currentState(false) {
    pinMode(_relayA, OUTPUT);
    pinMode(_relayB, OUTPUT);
    relayOff(_relayA);
    relayOff(_relayB);
}

void RelayValve::relayOn(uint8_t pin) {
    digitalWrite(pin, _activeLow ? LOW : HIGH);
}

void RelayValve::relayOff(uint8_t pin) {
    digitalWrite(pin, _activeLow ? HIGH : LOW);
}

void RelayValve::activate() {
    // Drop execution immediately if already running in an active state
    if (_currentState == true) {
        return;
    }

    if (_type == ValveType::SingleSolenoid) {
        relayOn(_relayA);
    } else if (_type == ValveType::DoubleSolenoid) {
        setPositionA();
    }

    _currentState = true;
}

void RelayValve::deactivate() {
    // Drop execution immediately if already running in an inactive state
    if (_currentState == false) {
        return;
    }

    if (_type == ValveType::SingleSolenoid) {
        relayOff(_relayA);
    } else if (_type == ValveType::DoubleSolenoid) {
        setPositionB();
    }

    _currentState = false;
}

void RelayValve::setPositionA() {
    if (_type != ValveType::DoubleSolenoid) {
        return;
    }
    relayOff(_relayB);
    relayOn(_relayA);
    delay(_pulseTimeMs);
    relayOff(_relayA);
}

void RelayValve::setPositionB() {
    if (_type != ValveType::DoubleSolenoid) {
        return;
    }
    relayOff(_relayA);
    relayOn(_relayB);
    delay(_pulseTimeMs);
    relayOff(_relayB);
}

void RelayValve::setPulseTime(uint16_t pulseTimeMs) {
    _pulseTimeMs = pulseTimeMs;
}

RelayValve::ValveType RelayValve::getType() const {
    return _type;
}

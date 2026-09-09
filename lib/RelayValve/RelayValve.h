#pragma once
#include <Arduino.h>

/**
 * @brief Controls pneumatic valves using Arduino relay outputs.
 */
class RelayValve {
public:
    enum class ValveType {
        SingleSolenoid,
        DoubleSolenoid
    };

    /**
     * @brief Construct a single-solenoid valve.
     */
    explicit RelayValve(uint8_t relayPin, bool activeLow = true);

    /**
     * @brief Construct a double-solenoid bistable valve.
     */
    RelayValve(uint8_t relayAPin, uint8_t relayBPin, uint16_t pulseTimeMs = 100, bool activeLow = true);

    void activate();
    void deactivate();
    void setPositionA();
    void setPositionB();
    void setPulseTime(uint16_t pulseTimeMs);
    ValveType getType() const;

private:
    void relayOn(uint8_t pin);
    void relayOff(uint8_t pin);

    ValveType _type;
    uint8_t _relayA;
    uint8_t _relayB;
    bool _activeLow;
    uint16_t _pulseTimeMs;
    bool _currentState; // <-- Tracks current position (false = Inactive/Pos B, true = Active/Pos A)
};

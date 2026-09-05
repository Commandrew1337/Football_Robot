#pragma once

#include <Arduino.h>

/**
 * @brief Controls pneumatic valves using Arduino relay outputs.
 *
 * Supports:
 * - Single-solenoid spring-return valves
 * - Double-solenoid bistable valves
 *
 * Designed for use with relay modules. By default,
 * active-low relay boards are assumed.
 */
class RelayValve
{
public:

    /**
     * @brief Supported valve types.
     */
    enum class ValveType
    {
        SingleSolenoid,
        DoubleSolenoid
    };

    /**
     * @brief Construct a single-solenoid valve.
     *
     * @param relayPin Relay output pin.
     * @param activeLow True for active-low relay modules.
     */
    explicit RelayValve(
        uint8_t relayPin,
        bool activeLow = true);

    /**
     * @brief Construct a double-solenoid bistable valve.
     *
     * @param relayAPin Relay output controlling coil A.
     * @param relayBPin Relay output controlling coil B.
     * @param pulseTimeMs Coil pulse duration in milliseconds.
     * @param activeLow True for active-low relay modules.
     */
    RelayValve(
        uint8_t relayAPin,
        uint8_t relayBPin,
        uint16_t pulseTimeMs = 100,
        bool activeLow = true);

    /**
     * @brief Energize a single-solenoid valve.
     *
     * For spring-return valves, this moves the valve
     * away from its default position.
     */
    void activate();

    /**
     * @brief De-energize a single-solenoid valve.
     *
     * For spring-return valves, this allows the valve
     * to return to its default position.
     */
    void deactivate();

    /**
     * @brief Move a double-solenoid valve to Position A.
     *
     * Pulses coil A for the configured pulse time.
     */
    void setPositionA();

    /**
     * @brief Move a double-solenoid valve to Position B.
     *
     * Pulses coil B for the configured pulse time.
     */
    void setPositionB();

    /**
     * @brief Set pulse duration for double-solenoid valves.
     *
     * @param pulseTimeMs Pulse duration in milliseconds.
     */
    void setPulseTime(uint16_t pulseTimeMs);

    /**
     * @brief Get the configured valve type.
     *
     * @return Valve type.
     */
    ValveType getType() const;

private:

    /**
     * @brief Turn a relay on.
     */
    void relayOn(uint8_t pin);

    /**
     * @brief Turn a relay off.
     */
    void relayOff(uint8_t pin);

    ValveType _type;

    uint8_t _relayA;
    uint8_t _relayB;

    bool _activeLow;

    uint16_t _pulseTimeMs;
};
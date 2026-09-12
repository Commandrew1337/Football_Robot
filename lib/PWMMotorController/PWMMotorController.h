#pragma once

#include <Arduino.h>
#include <Servo.h>

class PWMMotorController
{
public:
    enum class ControllerType
    {
        Victor888,
        Victor884,
        Jaguar,
        Talon,
        Spark,
        Generic
    };

    struct PWMCalibration
    {
        int fullForward;
        int deadbandHigh;
        int center;
        int deadbandLow;
        int fullReverse;
    };

    PWMMotorController(
        uint8_t pwmPin,
        ControllerType type,
        bool inverted = false);

    // FIX: Hardware initialization routine to call inside setup()
    void begin();

    void set(double output);
    void stop();

    void setInverted(bool inverted);
    bool getInverted() const;

    double get() const;
    ControllerType getType() const;

private:
    uint8_t _pin;
    ControllerType _type;
    bool _inverted;
    double _output = 0.0;
    Servo _servo;

    PWMCalibration getCalibration() const;
};

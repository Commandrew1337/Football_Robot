#include "PWMMotorController.h"

PWMMotorController::PWMMotorController(
    uint8_t pwmPin,
    ControllerType type,
    bool inverted)
    : _pin(pwmPin),
      _type(type),
      _inverted(inverted)
{
    _servo.attach(_pin);

    PWMCalibration cal = getCalibration();

    // Initialize to neutral.
    _servo.writeMicroseconds(cal.center);
}

PWMMotorController::PWMCalibration
PWMMotorController::getCalibration() const
{
    switch (_type)
    {
        case ControllerType::Victor888:
            return {
                2027, // Full Forward
                1525, // Deadband High
                1507, // Center
                1490, // Deadband Low
                1026  // Full Reverse
            };

        case ControllerType::Victor884:
            return {
                2027,
                1525,
                1507,
                1490,
                1026
            };

        case ControllerType::Jaguar:
            return {
                2310,
                1550,
                1507,
                1454,
                697
            };

        case ControllerType::Talon:
            return {
                2000,
                1500,
                1500,
                1500,
                1000
            };

        case ControllerType::Spark:
            return {
                2000,
                1500,
                1500,
                1500,
                1000
            };

        case ControllerType::Generic:
        default:
            return {
                2000,
                1500,
                1500,
                1500,
                1000
            };
    }
}

void PWMMotorController::set(double output)
{
    output = constrain(output, -1.0, 1.0);

    _output = output;

    if (_inverted)
    {
        output = -output;
    }

    PWMCalibration cal = getCalibration();

    int pwm;

    if (output > 0.0)
    {
        pwm =
            cal.deadbandHigh +
            static_cast<int>(
                output *
                (cal.fullForward - cal.deadbandHigh));
    }
    else if (output < 0.0)
    {
        pwm =
            cal.deadbandLow +
            static_cast<int>(
                (-output) *
                (cal.fullReverse - cal.deadbandLow));
    }
    else
    {
        pwm = cal.center;
    }

    _servo.writeMicroseconds(pwm);
}

void PWMMotorController::stop()
{
    PWMCalibration cal = getCalibration();

    _output = 0.0;

    _servo.writeMicroseconds(cal.center);
}

void PWMMotorController::setInverted(bool inverted)
{
    _inverted = inverted;
}

bool PWMMotorController::getInverted() const
{
    return _inverted;
}

double PWMMotorController::get() const
{
    return _output;
}

PWMMotorController::ControllerType
PWMMotorController::getType() const
{
    return _type;
}



/* Example Usage
#include <Arduino.h>
#include "PWMMotorController.h"

PWMMotorController leftMotor(
    5,
    PWMMotorController::ControllerType::Victor888);

PWMMotorController rightMotor(
    6,
    PWMMotorController::ControllerType::Victor888,
    true); // inverted

void setup()
{
}

void loop()
{
    leftMotor.set(0.50);
    rightMotor.set(0.50);

    delay(2000);

    leftMotor.stop();
    rightMotor.stop();

    delay(1000);
}
*/
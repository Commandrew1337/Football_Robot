#include "PWMMotorController.h"

PWMMotorController::PWMMotorController(
    uint8_t pwmPin,
    ControllerType type,
    bool inverted)
    : _pin(pwmPin),
      _type(type),
      _inverted(inverted)
{
    // Constructor kept clean of early global attachments
}

// Hardware initialization routine executed inside setup() after system timers wake up
void PWMMotorController::begin()
{
    PWMCalibration cal = getCalibration();
    
    // FIX: Pre-load the exact microsecond center value into the software structure 
    // BEFORE attaching the hardware pin. This ensures the timer register wakes up 
    // blasting exactly 1507us, preventing startup signal dips.
    _servo.writeMicroseconds(cal.center); 
    
    _servo.attach(_pin);
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
            // RoboRIO Profile Safety Bounds: Immune to serial parsing timing drift creeping
            return {
                2037, // Full Forward
                1539, // Deadband High 
                1507, // Center 
                1454, // Deadband Low  
                1026  // Full Reverse
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

    // CHANGED: Filter threshold widened from 0.005 (0.5%) to 0.025 (2.5%).
    // This cleanly swallows the 1% and 2% integer mapping steps (0.01, 0.02)
    // produced by raw gimbal drift and iBUS conversion math.
    if (abs(output) < 0.025)
    {
        output = 0.0;
    }

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
        // Safely locks down to exact calibration center (e.g. 1507us for Talon)
        pwm = cal.center;
    }

    // ADDED: HARDWARE LIMIT SHIELD
    // Ensures that even if a math overflow or variable data corruption 
    // forces 'pwm' out of bounds, the signal is physically locked 
    // within the Talon's safe limits before hitting the registers.
    pwm = constrain(pwm, cal.fullReverse, cal.fullForward);

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

#include "LaunchWheels.h"

LaunchWheels::LaunchWheels(
    PWMMotorController& leftA,
    PWMMotorController& leftB,
    PWMMotorController& rightA,
    PWMMotorController& rightB)
    : _leftA(leftA),
      _leftB(leftB),
      _rightA(rightA),
      _rightB(rightB),
      _enabled(false)
{
}

double LaunchWheels::clamp(double value)
{
    if (value < 0.0)
    {
        return 0.0;
    }

    if (value > 1.0)
    {
        return 1.0;
    }

    return value;
}

void LaunchWheels::enable()
{
    _enabled = true;
}

void LaunchWheels::disable()
{
    _enabled = false;
    stop();
}

void LaunchWheels::setSpeed(double speed)
{
    if (!_enabled)
    {
        stop();
        return;
    }

    // Launcher wheels are unidirectional.
    // Negative values are clamped to zero.
    speed = clamp(speed);

    _leftA.set(speed);
    _leftB.set(speed);

    _rightA.set(speed);
    _rightB.set(speed);
}

void LaunchWheels::launch(
    RcController& controller,
    byte speedChannel)
{
    if (!_enabled)
    {
        stop();
        return;
    }

    int rawSpeed =
        controller.readChannel(
            speedChannel,
            -100,
            100,
            0);

    // Map joystick:
    // -100 = 0% speed
    //    0 = 50% speed
    // +100 = 100% speed
    double speed = (rawSpeed + 100) / 200.0;

    setSpeed(speed);
}

void LaunchWheels::stop()
{
    _leftA.stop();
    _leftB.stop();

    _rightA.stop();
    _rightB.stop();
}

bool LaunchWheels::isEnabled() const
{
    return _enabled;
}
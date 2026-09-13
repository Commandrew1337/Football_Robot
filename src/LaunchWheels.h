#ifndef LAUNCHWHEELS_H
#define LAUNCHWHEELS_H

#include <Arduino.h>

#include "PWMMotorController.h"
#include "RcController.h"

class LaunchWheels
{
public:
    LaunchWheels(
        PWMMotorController& leftA,
        PWMMotorController& leftB,
        PWMMotorController& rightA,
        PWMMotorController& rightB);

    void enable();
    void disable();

    void setSpeed(double speed);

    void launch(
        RcController& controller,
        byte speedChannel);

    void stop();

    bool isEnabled() const;

private:
    double clamp(double value);

    PWMMotorController& _leftA;
    PWMMotorController& _leftB;
    PWMMotorController& _rightA;
    PWMMotorController& _rightB;

    bool _enabled;
};

#endif
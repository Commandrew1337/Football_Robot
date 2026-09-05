#pragma once

#include <Arduino.h>
#include "PWMMotorController.h"
#include "RcController.h"

class RobotDrive
{
public:
    RobotDrive(
        PWMMotorController& leftFront,
        PWMMotorController& leftRear,
        PWMMotorController& rightFront,
        PWMMotorController& rightRear,
        double joystickDeadband = 0.05,
        bool invertForward = false,
        bool invertTurn = false);

    void drive(
        RcController& controller,
        byte throttleChannel,
        byte turnChannel);

    void arcadeDrive(
        double forward,
        double turn);

    void stop();

private:
    PWMMotorController& _leftFront;
    PWMMotorController& _leftRear;
    PWMMotorController& _rightFront;
    PWMMotorController& _rightRear;

    double _deadband;
    bool _invertForward;
    bool _invertTurn;

    double clamp(double value);
    double applyDeadband(double value);
};
#include "RobotDrive.h"

RobotDrive::RobotDrive(
    PWMMotorController& leftFront,
    PWMMotorController& leftRear,
    PWMMotorController& rightFront,
    PWMMotorController& rightRear,
    double joystickDeadband,
    bool invertForward,
    bool invertTurn)
    : _leftFront(leftFront),
      _leftRear(leftRear),
      _rightFront(rightFront),
      _rightRear(rightRear),
      _deadband(joystickDeadband),
      _invertForward(invertForward),
      _invertTurn(invertTurn)
{
}

double RobotDrive::clamp(double value)
{
    if (value > 1.0)
    {
        return 1.0;
    }

    if (value < -1.0)
    {
        return -1.0;
    }

    return value;
}

double RobotDrive::applyDeadband(double value)
{
    if (abs(value) < _deadband)
    {
        return 0.0;
    }

    return value;
}

void RobotDrive::drive(
    RcController& controller,
    byte throttleChannel,
    byte turnChannel)
{
    double forward =
        controller.readChannel(
            throttleChannel,
            -100,
            100,
            0) / 100.0;

    double turn =
        controller.readChannel(
            turnChannel,
            -100,
            100,
            0) / 100.0;

    arcadeDrive(
        forward,
        turn);
}

void RobotDrive::arcadeDrive(
    double forward,
    double turn)
{
    if (_invertForward)
    {
        forward = -forward;
    }

    if (_invertTurn)
    {
        turn = -turn;
    }

    forward = applyDeadband(forward);
    turn = applyDeadband(turn);

    double left =
        clamp(forward + turn);

    double right =
        clamp(forward - turn);

    _leftFront.set(left);
    _leftRear.set(left);

    _rightFront.set(right);
    _rightRear.set(right);
}

void RobotDrive::stop()
{
    _leftFront.stop();
    _leftRear.stop();

    _rightFront.stop();
    _rightRear.stop();
}


/*
===============================================================================
Example 1 - Basic Robot
===============================================================================

#include "PWMMotorController.h"
#include "RobotDrive.h"
#include "RcController.h"

PWMMotorController leftFront(
    5,
    PWMMotorController::ControllerType::Victor888);

PWMMotorController leftRear(
    6,
    PWMMotorController::ControllerType::Victor888);

PWMMotorController rightFront(
    7,
    PWMMotorController::ControllerType::Victor888,
    true);

PWMMotorController rightRear(
    8,
    PWMMotorController::ControllerType::Victor888,
    true);

RobotDrive drive(
    leftFront,
    leftRear,
    rightFront,
    rightRear);

RcController controller(
    Serial1,
    Serial2,
    Serial);

void setup()
{
    controller.begin();
}

void loop()
{
    controller.update();

    if (!controller.isReceiverHardwareConnected())
    {
        drive.stop();
        return;
    }

    if (controller.isReadyToProcess())
    {
        drive.drive(
            controller,
            1, // Forward/Reverse channel
            0  // Turn channel
        );
    }
}


===============================================================================
Example 2 - Custom Deadband
===============================================================================

RobotDrive drive(
    leftFront,
    leftRear,
    rightFront,
    rightRear,
    0.10); // 10% joystick deadband


===============================================================================
Example 3 - Invert Forward Axis
===============================================================================

RobotDrive drive(
    leftFront,
    leftRear,
    rightFront,
    rightRear,
    0.05,
    true,   // invert forward
    false); // normal turn


===============================================================================
Example 4 - Invert Turn Axis
===============================================================================

RobotDrive drive(
    leftFront,
    leftRear,
    rightFront,
    rightRear,
    0.05,
    false,  // normal forward
    true);  // invert turn


===============================================================================
Example 5 - Invert Both Axes
===============================================================================

RobotDrive drive(
    leftFront,
    leftRear,
    rightFront,
    rightRear,
    0.08,
    true,
    true);


===============================================================================
Example 6 - Direct Arcade Drive
===============================================================================

void loop()
{
    drive.arcadeDrive(
        0.50,  // 50% forward
        0.25); // 25% right turn
}


===============================================================================
Example 7 - Full Forward
===============================================================================

drive.arcadeDrive(
    1.0,
    0.0);


===============================================================================
Example 8 - Full Reverse
===============================================================================

drive.arcadeDrive(
    -1.0,
    0.0);


===============================================================================
Example 9 - Spin Right
===============================================================================

drive.arcadeDrive(
    0.0,
    1.0);


===============================================================================
Example 10 - Spin Left
===============================================================================

drive.arcadeDrive(
    0.0,
    -1.0);


===============================================================================
Example 11 - Emergency Stop
===============================================================================

drive.stop();


===============================================================================
Typical FlySky Channel Mapping
===============================================================================
Typical single-stick arcade setup:

drive.drive(
    controller,
    1, // throttle
    0  // steering
);

===============================================================================
*/
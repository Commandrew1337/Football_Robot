#pragma once

#include <Arduino.h>
#include <Servo.h>

/**
 * @brief Wrapper for FRC-style PWM motor controllers.
 *
 * Supports:
 * - Talon (PWM)
 * - Victor 888
 * - Victor 884
 * - Jaguar
 * - Spark
 * - Generic PWM controllers
 *
 * Commands are specified as values from -1.0 to +1.0.
 */
class PWMMotorController
{
public:

    /**
     * @brief Supported PWM controller types.
     */
    enum class ControllerType
    {
        Talon,
        Victor888,
        Victor884,
        Jaguar,
        Spark,
        Generic
    };

    /**
     * @brief Construct a motor controller.
     *
     * The PWM output is automatically attached and initialized
     * to the controller's neutral position.
     *
     * @param pwmPin Arduino pin connected to the controller PWM input.
     * @param type Motor controller type.
     * @param inverted True to invert motor direction.
     */
    explicit PWMMotorController(
        uint8_t pwmPin,
        ControllerType type = ControllerType::Generic,
        bool inverted = false);

    /**
     * @brief Set motor output.
     *
     * Valid range is -1.0 to +1.0.
     *
     * -1.0 = Full reverse
     *  0.0 = Neutral
     * +1.0 = Full forward
     *
     * Values outside the range are automatically clamped.
     *
     * @param output Desired motor output.
     */
    void set(double output);

    /**
     * @brief Command the controller to neutral.
     */
    void stop();

    /**
     * @brief Change motor inversion.
     *
     * @param inverted True to invert motor direction.
     */
    void setInverted(bool inverted);

    /**
     * @brief Get inversion state.
     *
     * @return True if inverted.
     */
    bool getInverted() const;

    /**
     * @brief Get the last commanded output value.
     *
     * @return Output value from -1.0 to +1.0.
     */
    double get() const;

    /**
     * @brief Get controller type.
     *
     * @return Configured controller type.
     */
    ControllerType getType() const;

private:

    struct PWMCalibration
    {
        int fullForward;
        int deadbandHigh;
        int center;
        int deadbandLow;
        int fullReverse;
    };

    /**
     * @brief Get PWM calibration for controller type.
     */
    PWMCalibration getCalibration() const;

    Servo _servo;

    uint8_t _pin;
    ControllerType _type;

    bool _inverted;
    double _output = 0.0;
};
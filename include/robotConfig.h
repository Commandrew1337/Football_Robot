#pragma once

// ============================================================================
// SYSTEM SETTINGS
// ============================================================================

// ============================================================================
// HARDWARE PIN CONFIGURATION (For Arduino Mega 2560)
// ============================================================================
// Tank Drive Motors (Left side)
#define PIN_MOTOR_L_PWM     2       // Speed control (PWM pin)

// Tank Drive Motors (Right side)
#define PIN_MOTOR_R_PWM     3       // Speed control (PWM pin)


// Football Flywheel Shooter Mechanisms
#define PIN_SHOOTER_L_PWM   4       // Left flywheel speed (PWM pin)
#define PIN_SHOOTER_R_PWM   5       // Right flywheel speed (PWM pin)
#define PIN_FEEDER_SERVO    9       // Servo pin that pushes football into flywheels

// ============================================================================
// ROBOT TUNING & CALIBRATION CONSTANTS
// ============================================================================
// Drive Tuning
#define MAX_DRIVE_SPEED     255     // Absolute limit for PWM (0-255)
#define MIN_DRIVE_SPEED     30      // Minimum power needed to overcome friction

// Shooter Tuning
#define SHOOTER_IDLE_SPEED  50      // Low speed to keep flywheels spinning softly
#define SHOOTER_MAX_SPEED   255     // Full power for long-distance football shots
#define FEED_SERVO_IDLE     10      // Angle (degrees) where the feeder is retracted
#define FEED_SERVO_PUSH     90      // Angle (degrees) to shove football into flywheels


// ============================================================================
// iBUS Controller Channel Mapping
// ============================================================================
#define CH_THROTTLE         3       // Left stick Y-axis
#define CH_PITCH            2       // Right stick Y-axis
#define CH_YAW              4       // Left stick X-axis
#define CH_ROLL             1       // Left stick X-axis
#define CH_SWA              5
#define CH_SWB              6
#define CH_VRA              7
#define CH_VRB              8
#define CH_SWC              9
#define CH_SWD              10


/* =========================================================================================
 *                          ARDUINO MEGA 2560 - SERIAL PINOUT MAP
 * =========================================================================================
 * 
 *  [SERIAL] -> HARDWARE USB INTERFACE (PC DEBUGGING)
 *  ----------------------------------------------------------------------------------------
 *  - Purpose: For sending live diagnostic tracking data to your computer's Serial Monitor.
 *  - Pins:    Uses internal ATMega16U2 chip mapped directly to the USB connection.
 *  - Speed:   Configured at 115200 Baud rate for lightweight, high-performance data streaming.
 * 
 *  [SERIAL1] -> iBUS RECEIVER OUT (STICK CHANNELS INPUT)
 *  ----------------------------------------------------------------------------------------
 *  - Purpose: For receiving live raw joystick data streams coming from your FlySky Receiver.
 *  - Pin 19 (RX1) <===== Connects to: iBUS "Servo" or "Out" data pin on the FlySky Receiver.
 *  - Pin 18 (TX1) <===== LEAVE UNCONNECTED. (This connection only listens for data frames).
 * 
 *  [SERIAL2] -> iBUS SENSOR Telemetry (HALF-DUPLEX TELEMETRY DATA LOOP)
 *  ----------------------------------------------------------------------------------------
 *  - Purpose: For bi-directional telemetry communication (sending Ex.V Battery data to TX).
 *  - Pin 16 (RX2) <---┐
 *                     ├── [ 1.2k Ohm Resistor ] <== Connects to: FlySky "Sens" (SENS) Port.
 *  - Pin 17 (TX2) <---┘
 * 
 *  * ELECTRICAL NOTE FOR SERIAL2 TELEMETRY: 
 *    The FlySky telemetry port uses a single wire for both transmitting and receiving data 
 *    (Half-Duplex). To share this wire safely on the Mega's split RX2/TX2 system, bridge Pin 16 
 *    and Pin 17 together using a 1.2k Ohm resistor, then run the wire out from Pin 16 (RX2) 
 *    directly into the Receiver's SENS pin.
 * ========================================================================================= */
#pragma once

namespace robotConfig {
    // ============================================================================
    // SYSTEM SETTINGS
    // ============================================================================

    // ============================================================================
    // HARDWARE PIN CONFIGURATION (For Arduino Mega 2560)
    // ============================================================================
    // "byte" = "uint8_t"

    const uint8_t RELAY_IN1 = 24;
    const uint8_t RELAY_IN2 = 25;
    const uint8_t RELAY_IN3 = 26;
    const uint8_t RELAY_IN4 = 27;

    constexpr uint8_t COMPRESSOR_PRESSURE_SWITCH = 8;
    constexpr uint8_t COMPRESSOR_SPIKE_REVERSE = 9;
    constexpr uint8_t COMPRESSOR_SPIKE_FORWARD = 10;

    constexpr uint8_t HORN_SPIKE_REVERSE = 13;
    constexpr uint8_t HORN_SPIKE_FORWARD = 12;

    constexpr uint8_t RSL_PIN = 11;

    constexpr uint8_t MLF = 2;
    constexpr uint8_t MLR = 3;
    constexpr uint8_t MRF = 4;
    constexpr uint8_t MRR = 5;
    constexpr uint8_t MT1 = 6;
    constexpr uint8_t MT2 = 7;
    constexpr uint8_t MT3 = 15;
    constexpr uint8_t MT4 = 14;    

    // ============================================================================
    // ROBOT TUNING & CALIBRATION CONSTANTS
    // ============================================================================
    // Drive Tuning
    // #define MAX_DRIVE_SPEED     255     // Absolute limit for PWM (0-255)
    // #define MIN_DRIVE_SPEED     30      // Minimum power needed to overcome friction

    // Shooter Tuning
    // #define SHOOTER_IDLE_SPEED  50      // Low speed to keep flywheels spinning softly
    // #define SHOOTER_MAX_SPEED   255     // Full power for long-distance football shots
    // #define FEED_SERVO_IDLE     10      // Angle (degrees) where the feeder is retracted
    // #define FEED_SERVO_PUSH     90      // Angle (degrees) to shove football into flywheels


    // ============================================================================
    // iBUS Controller Channel Mapping (Shifted down by 1 for 0-indexing)
    // ============================================================================
    constexpr uint8_t CH_ROLL     = 0;  // Physical Ch 1 (Right Stick X-axis)
    constexpr uint8_t CH_PITCH    = 1;  // Physical Ch 2 (Right Stick Y-axis)
    constexpr uint8_t CH_THROTTLE = 2;  // Physical Ch 3 (Left Stick Y-axis)
    constexpr uint8_t CH_YAW      = 3;  // Physical Ch 4 (Left Stick X-axis)
    
    constexpr uint8_t CH_SWA      = 4;  // Physical Ch 5 (Switch A)
    constexpr uint8_t CH_SWB      = 5;  // Physical Ch 6 (Switch B)
    constexpr uint8_t CH_VRA      = 6;  // Physical Ch 7 (Dial A)
    constexpr uint8_t CH_VRB      = 7;  // Physical Ch 8 (Dial B)
    constexpr uint8_t CH_SWC      = 8;  // Physical Ch 9 (Switch C)
    constexpr uint8_t CH_SWD      = 9;  // Physical Ch 10 (Switch D)



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
}
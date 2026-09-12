#pragma once

namespace robotConfig {
    // ============================================================================
    // ⚠️ ARDUINO MEGA 2560 PIN SAFETY & RESERVATION WARNINGS
    // ============================================================================
    // Before adding new pins, verify against these critical hardware rules:
    //
    // 🛑 DANGEROUS BOOTLOADER PINS (Do Not Use for Actuators/Relays)
    //   - Pin 13 : Flashes rapidly at 50Hz+ during code uploads (Built-in 'L' LED).
    //              Will fry relay contacts and cause dangerous accidental movements.
    //
    // 🛑 HARDWARE SERIAL RESERVATIONS (Do Not Use for Digital I/O or PWM)
    //   - Pins 0  & 1  (Serial0) : Reserved for PC USB programming & Serial Monitor.
    //   - Pins 18 & 19 (Serial1) : Reserved for your FlySky iBUS Receiver Input.
    //   - Pins 16 & 17 (Serial2) : Reserved for your FlySky Telemetry Loop.
    //   - Pins 14 & 15 (Serial3) : Keep EMPTY. Used for serial loops.
    //
    // ⚡ HARDWARE PWM TIMERS & COMPATIBILITY RULES
    //   - Because we switched to 'Servo_Hardware_PWM.h' to eliminate background 
    //     serial jitter, you MUST use pins hardwired to 16-bit Hardware Timers.
    //   - NATIVE TIMERS SEIZED: Timer 3 (Pins 2, 3, 5), Timer 4 (Pins 6, 7, 8), 
    //                           and Timer 5 (Pins 44, 45, 46).
    //   - 🛑 PIN 4 INCOMPATIBILITY WARNING: Pin 4 runs on an 8-bit clock (Timer 0) 
    //     and CANNOT generate high-precision 16-bit hardware waves. Right Front 
    //     Motor (MRF) was moved to Pin 46 to protect drivetrain symmetry.
    //   - 🛑 PIN 8 HARWARE TIMING RESERVATION: Pin 8 is seized by Timer 4. The 
    //     Compressor Pressure Switch was moved to Pin 28 to prevent data clashes.
    //
    // 🔌 ACTIVE-LOW RELAY BANK STARTUP SAFETY
    //   - Pins 24-27 float LOW during power-on/boot text flushes.
    //   - Ensure your setup() forces these HIGH before declaring them OUTPUTs
    //     to prevent your compressor or valves from popping at startup.
    // ============================================================================


    // ============================================================================
    // HARDWARE PIN CONFIGURATION (For Arduino Mega 2560)
    // ============================================================================
    // "byte" = "uint8_t"

    // --- SAFE DIGITAL PERIPHERALS (NO CONFLICTS) ---
    const uint8_t RELAY_IN1 = 24;
    const uint8_t RELAY_IN2 = 25;
    const uint8_t RELAY_IN3 = 26;
    const uint8_t RELAY_IN4 = 27;

    constexpr uint8_t HORN_SPIKE_REVERSE = 22;
    constexpr uint8_t HORN_SPIKE_FORWARD = 12;
    constexpr uint8_t RSL_PIN = 11;

    // --- COMPRESSOR REMAP (CLEAR OF TIMER SLOTS) ---
    // Moved from Pin 8 to Pin 28 to leave Pin 8 completely open for Timer 4 servo outputs
    constexpr uint8_t COMPRESSOR_PRESSURE_SWITCH = 28; 
    constexpr uint8_t COMPRESSOR_SPIKE_REVERSE = 9;
    constexpr uint8_t COMPRESSOR_SPIKE_FORWARD = 10;

    // --- DRIVETRAIN REMAP (100% PURE HARDWARE TIMERS) ---
    // Fully immune to background serial interrupts and idle jumping/twitching
    constexpr uint8_t MLF = 2;   // Native Hardware Timer 3 Channel B
    constexpr uint8_t MLR = 3;   // Native Hardware Timer 3 Channel A
    constexpr uint8_t MRF = 46;  // Native Hardware Timer 5 Channel A (Moved from Pin 4)
    constexpr uint8_t MRR = 5;   // Native Hardware Timer 3 Channel C

    // --- AUXILIARY TOOL MOTOR ASSIGNMENTS ---
    // Fully immune to background serial interrupts and idle jumping/twitching
    constexpr uint8_t MT1 = 6;   // Native Hardware Timer 4 Channel A
    constexpr uint8_t MT2 = 7;   // Native Hardware Timer 4 Channel B
    constexpr uint8_t MT3 = 45;  // Native Hardware Timer 5 Channel B
    constexpr uint8_t MT4 = 44;  // Native Hardware Timer 5 Channel C 

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

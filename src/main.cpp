#include <Arduino.h>
#include "RcController.h"

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
RcController FScontroller(Serial1, Serial2, Serial);

void setup() {
  Serial.begin(115200);
  FScontroller.begin();
}

void loop() {
  FScontroller.update(); // Keep background telemetry and serial caching alive

  // HARDWARE SAFEGUARD: Stops code execution instantly if the physical iBUS cable 
  // shakes loose or loses power, preventing a dangerous runaway robot scenario!
  if (!FScontroller.isReceiverHardwareConnected()) {
    // ==> PLACE SYSTEM KILL / EMERGENCY BRAKING COMMANDS HERE <==
    return; 
  }

  // Execute operations within the steady 20ms frame interval
  if (FScontroller.isReadyToProcess()) {
    
    // Read and interpret stick movements smoothly.
    // If transmitter is off, these will automatically return whatever default failsafe 
    // values you configured directly inside your FlySky Transmitter setup menu.
    int throttle = FScontroller.readChannel(2, -255, 255, 0); 
    int steering = FScontroller.readChannel(0, -255, 255, 0); 

    // ==> EXECUTE DRIVING OUTPUT SCHEDULERS HERE <==

    // Feed current system voltage reading back to FScontroller screen
    int liveBatteryVolt = 1240; 
    FScontroller.sendBatteryVoltage(liveBatteryVolt);

    // Call diagnostic tool safely without introducing motor stuttering lags
    FScontroller.printDebugChannels(); 
  }
}

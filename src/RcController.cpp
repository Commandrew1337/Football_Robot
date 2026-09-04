#include "RcController.h"

// Constructor implementation linking hardware references
RcController::RcController(HardwareSerial& rcSer, HardwareSerial& sensSer, HardwareSerial& dbgSer) 
  : rcSerial(rcSer), sensSerial(sensSer), debugSerial(dbgSer), 
    lastExecutionTime(0), lastPrintTime(0), lastWarningTime(0), lastValidPacketTime(0) {}

// Initializes the iBUS subsystems
void RcController::begin() {
  ibusRc.begin(rcSerial);
  ibusSens.begin(sensSerial, IBUSBM_NOTIMER); // IBUSBM_NOTIMER prevents timer conflicts
  ibusSens.addSensor(IBUSS_EXTV);            // Register Slot 1 (Index 0) for External Voltage
  debugSerial.println("iBus Controller Object Initialized Successfully.");
}

// Background handler that must be called unfiltered at top-level loop speed
void RcController::update() {
  ibusSens.loop(); // Pulls raw background serial data for library caching
}

// Watchdog connection check to verify if the physical receiver wire is plugged in and powered
bool RcController::isReceiverHardwareConnected() {
  // Read channel 1 to see if data packets are flowing into the Arduino registers
  uint16_t rawChannel1 = ibusRc.readChannel(0);

  // If a packet has arrived, reset our connection watchdog timer.
  if (rawChannel1 != 0) {
    lastValidPacketTime = millis();
  }

  // Watchdog Timeout Check.
  // If the Arduino hasn't seen a new serial frame in 60ms, the receiver itself is unplugged or dead.
  if (millis() - lastValidPacketTime > 60) {
    if (millis() - lastWarningTime >= warningInterval) {
      debugSerial.println("CRITICAL: Receiver Disconnected / Wire Unplugged!");
      lastWarningTime = millis();
    }
    return false;
  }

  return true; // The connection to the receiver is physically healthy
}

// Fixed-interval timing gate acting as a system metronome
bool RcController::isReadyToProcess() {
  if (millis() - lastExecutionTime >= executionInterval) {
    lastExecutionTime = millis(); 
    return true;                  
  }
  return false;                   
}

// Sends millivolt calculation back down the iBUS line to display on transmitter telemetry field 0
void RcController::sendBatteryVoltage(int millivolts) {
  ibusSens.setSensorMeasurement(1, millivolts);
}

// Reads stick state and clamps output constraints dynamically
int RcController::readChannel(byte channelInput, int minLimit, int maxLimit, int defaultValue) {
  uint16_t ch = ibusRc.readChannel(channelInput);
  if (ch == 0) return defaultValue;
  
  int mappedValue = map(ch, 1000, 2000, minLimit, maxLimit);
  return constrain(mappedValue, minLimit, maxLimit);
}

// Returns state map for binary switches
bool RcController::readSwitch(byte channelInput, bool defaultValue) {
  int intDefaultValue = defaultValue ? 100 : 0;
  int ch = readChannel(channelInput, 0, 100, intDefaultValue);
  return (ch > 50);
}

// Evaluates 3-way toggle switch signal bounds matching physically native FlySky ranges
Switch3Way RcController::read3WaySwitch(byte channelInput, Switch3Way defaultValue) {
  uint16_t ch = ibusRc.readChannel(channelInput);
  if (ch == 0) return defaultValue;

  if (ch > 1750) return SWITCH_UP;
  if (ch >= 1250 && ch <= 1750) return SWITCH_MID;
  return SWITCH_DOWN;
}

// Internally timed console logger protecting the MCU cycle rate from heavy string buffering overhead
void RcController::printDebugChannels() {
  if (millis() - lastPrintTime >= printInterval) {
    lastPrintTime = millis();

    for (int i = 0; i < 10; i++) {
      debugSerial.print("Ch");
      debugSerial.print(i + 1);
      debugSerial.print(": ");
      debugSerial.print(readChannel(i, -100, 100, 0));
      debugSerial.print("\t");
    }
    debugSerial.println();
  }
}

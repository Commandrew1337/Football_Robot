#include "RcController.h"

// Constructor implementation linking hardware references
RcController::RcController(HardwareSerial& rcSer, HardwareSerial& sensSer, HardwareSerial& dbgSer) 
  : rcSerial(rcSer), sensSerial(sensSer), debugSerial(dbgSer), 
    lastExecutionTime(0), lastPrintTime(0), lastWarningTime(0), lastValidPacketTime(0) 
{
  // Seed physical resting-state default baselines at startup
  _lastValidMappedValues[0] = 0;    // Ch1: Roll / Steering Stick (Center)
  _lastValidMappedValues[1] = 0;    // Ch2: Pitch / Throttle Stick (Center)
  _lastValidMappedValues[2] = -100; // Ch3: Main Throttle Stick (Absolute Bottom)
  _lastValidMappedValues[3] = 0;    // Ch4: Yaw / Steering Stick (Center)
  
  // Channels 5 through 10 (Switches A, B, C, D and Analog Dials A, B) default safely to minimum
  for (int i = 4; i < 10; i++) {
    _lastValidMappedValues[i] = -100;
  }
}

// Initializes the iBUS subsystems
void RcController::begin() {
  ibusRc.begin(rcSerial);
  ibusSens.begin(sensSerial, IBUSBM_NOTIMER); // IBUSBM_NOTIMER prevents timer conflicts
  ibusSens.addSensor(IBUSS_EXTV);            // Register Slot 1 (Index 0) for External Voltage
  debugSerial.println("iBus Controller Object Initialized Successfully.");
}

// Background handler that must be called unfiltered at top-level loop speed
void RcController::update() {
  ibusRc.loop();   // Processes incoming stick movements
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
  if (channelInput >= 10) return defaultValue;

  uint16_t ch = ibusRc.readChannel(channelInput);
  
  // FAILSAFE MITIGATION: If a background frame drops (returns 0), smoothly supply 
  // the last successfully cached position instead of forcing a sudden zero-drop.
  if (ch == 0) {
    return _lastValidMappedValues[channelInput];
  }
  
  int mappedValue = map(ch, 1000, 2000, minLimit, maxLimit);
  mappedValue = constrain(mappedValue, minLimit, maxLimit);
  
  // Update internal cache memory with this valid value
  _lastValidMappedValues[channelInput] = mappedValue;
  
  return mappedValue;
}

// Returns state map for binary switches
bool RcController::readSwitch(byte channelInput, bool defaultValue) {
  // Force default ranges and limits to evaluate within a clean -100 to 100 spectrum
  int intDefaultValue = defaultValue ? 100 : -100;
  int ch = readChannel(channelInput, -100, 100, intDefaultValue);
  
  // Symmetrical mid-point evaluation crossing zero
  return (ch > 0);
}

// Evaluates 3-way toggle switch signal bounds matching physically native FlySky ranges
Switch3Way RcController::read3WaySwitch(byte channelInput, Switch3Way defaultValue) {
  uint16_t ch = ibusRc.readChannel(channelInput);
  
  // Route failsafe through the cache memory to maintain structural continuity
  if (ch == 0) {
    int cachedVal = _lastValidMappedValues[channelInput];
    if (cachedVal > 33) return SWITCH_DOWN;
    if (cachedVal >= -33 && cachedVal <= 33) return SWITCH_MID;
    return SWITCH_UP;
  }

  if (ch > 1750) return SWITCH_DOWN;
  if (ch >= 1250 && ch <= 1750) return SWITCH_MID;
  return SWITCH_UP;
}

// Internally timed console logger protecting the MCU cycle rate from heavy string buffering overhead
void RcController::printDebugChannels() {
  if (millis() - lastPrintTime >= printInterval) {
    lastPrintTime = millis();

    for (int i = 0; i < 10; i++) {
      debugSerial.print("Ch");
      debugSerial.print(i + 1);
      debugSerial.print(": ");
      // Dynamically pass the channel's custom resting cache array value as the fallback tracker target
      debugSerial.print(readChannel(i, -100, 100, _lastValidMappedValues[i])); 
      debugSerial.print("\t");
    }
    debugSerial.println();
  }
}

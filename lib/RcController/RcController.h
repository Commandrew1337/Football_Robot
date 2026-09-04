#pragma once

#include <Arduino.h>
#include <IBusBM.h>

// Define easy-to-read names for your 3-way switch positions
enum Switch3Way { SWITCH_UP, SWITCH_MID, SWITCH_DOWN };

class RcController {
  private:
    IBusBM ibusRc;
    IBusBM ibusSens;
    HardwareSerial& rcSerial;
    HardwareSerial& sensSerial;
    HardwareSerial& debugSerial;

    // --- TIMING VARIABLES ---
    unsigned long lastExecutionTime;
    const unsigned long executionInterval = 20; // 20ms iBUS frame rate metronome

    unsigned long lastPrintTime;
    const unsigned long printInterval = 200;    // Limit PC printing to 200ms

    unsigned long lastWarningTime;
    const unsigned long warningInterval = 500;   // Limit disconnect warnings to 500ms
    
    unsigned long lastValidPacketTime;          // Watchdog timer tracking for physical cable health

  public:
    // Constructor
    RcController(HardwareSerial& rcSer, HardwareSerial& sensSer, HardwareSerial& dbgSer);

    // Public API Methods
    void begin();
    void update();
    bool isReceiverHardwareConnected(); // Renamed to accurately reflect its hardware checking purpose
    bool isReadyToProcess();
    void sendBatteryVoltage(int millivolts);
    
    // Channel Reading Methods
    int readChannel(byte channelInput, int minLimit, int maxLimit, int defaultValue);
    bool readSwitch(byte channelInput, bool defaultValue);
    Switch3Way read3WaySwitch(byte channelInput, Switch3Way defaultValue);
    
    // Performance Protected Diagnostic Output
    void printDebugChannels();
};

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
    
    /* 
     * WHY 20ms? 
     * 1. Your FlySky transmitter broadcasts a new radio packet over the air exactly every 20ms (50Hz).
     * 2. Checking sticks faster is pointless because the radio data hasn't updated yet.
     * 3. Standard RC ESCs, motor drivers, and servos expect a steady 50Hz (20ms) control heartbeat.
     */
    const unsigned long executionInterval = 20; 

    // Slow internal timer just for text printing (printing text is slow and lags motors)
    unsigned long lastPrintTime;
    const unsigned long printInterval = 200;    // Limit PC printing to every 200ms

    unsigned long lastWarningTime;
    const unsigned long warningInterval = 500;   // Limit disconnect warnings to every 500ms

  public:
    // Constructor passes the hardware serial ports by reference
    RcController(HardwareSerial& rcSer, HardwareSerial& sensSer, HardwareSerial& dbgSer) 
      : rcSerial(rcSer), sensSerial(sensSer), debugSerial(dbgSer), 
        lastExecutionTime(0), lastPrintTime(0), lastWarningTime(0) {}

    // Initializes the iBUS subsystems
    void begin() {
      ibusRc.begin(rcSerial);
      ibusSens.begin(sensSerial, IBUSBM_NOTIMER); // IBUSBM_NOTIMER prevents timer conflicts
      ibusSens.addSensor(IBUSS_EXTV);            // Register Slot 1 (Index 0) for External Voltage
      debugSerial.println("iBus Controller Object Initialized Successfully.");
    }

    // This background worker MUST run at raw, maximum speed with absolutely zero delays
    void update() {
      ibusSens.loop(); // Instantly handles telemetry polling pulses sent from the receiver
    }

    // Checks connection status. True = Online, False = Signal Lost
    bool isConnected() {
      if (ibusRc.readChannel(0) == 0) {
        if (millis() - lastWarningTime >= warningInterval) {
          debugSerial.println("WARNING: Signal Lost / Transmitter Disconnected!");
          lastWarningTime = millis();
        }
        return false;
      }
      return true;
    }

    /*
     * THE NON-BLOCKING METRONOME
     * Instead of freezing the chip like delay(20), this function works like a stopwatch.
     * It allows the main loop to run millions of times per second to keep background telemetry alive,
     * but only lets your heavy driving/printing calculations pass through once every 20ms.
     */
    bool isReadyToProcess() {
      if (millis() - lastExecutionTime >= executionInterval) {
        lastExecutionTime = millis(); // Reset stopwatch for the next 20ms cycle
        return true;                  // Open the gate! Run the code inside the 'if' block
      }
      return false;                   // Gate closed. Bypass the block and keep looping
    }

    // Updates telemetry data on your transmitter screen (Index 0 matches registered sensor)
    void sendBatteryVoltage(int millivolts) {
      ibusSens.setSensorMeasurement(0, millivolts);
    }

    // Read a specific stick/dial channel and map it to custom hardware limits
    int readChannel(byte channelInput, int minLimit, int maxLimit, int defaultValue) {
      uint16_t ch = ibusRc.readChannel(channelInput);
      if (ch == 0) return defaultValue;
      
      int mappedValue = map(ch, 1000, 2000, minLimit, maxLimit);
      return constrain(mappedValue, minLimit, maxLimit);
    }

    // Read a standard 2-way switch and return a boolean
    bool readSwitch(byte channelInput, bool defaultValue) {
      int intDefaultValue = defaultValue ? 100 : 0;
      int ch = readChannel(channelInput, 0, 100, intDefaultValue);
      return (ch > 50);
    }

    // Read a 3-way switch (like SwC) and return UP, MID, or DOWN state
    Switch3Way read3WaySwitch(byte channelInput, Switch3Way defaultValue) {
      uint16_t ch = ibusRc.readChannel(channelInput);
      if (ch == 0) return defaultValue;

      if (ch > 1750) return SWITCH_UP;
      if (ch >= 1250 && ch <= 1750) return SWITCH_MID;
      return SWITCH_DOWN;
    }

    /*
     * PROTECTED PRINTING
     * Printing strings to a PC takes a massive amount of processing time. 
     * This function uses an internal 200ms timer so it automatically skips heavy printing 
     * 9 out of 10 times, ensuring your rapid motor calculations never experience lag.
     */
    void printDebugChannels() {
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
};

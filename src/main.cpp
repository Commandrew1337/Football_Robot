#include <Arduino.h>
#include <IBusBM.h>

IBusBM ibusRc;
HardwareSerial& debugSerial = Serial;
HardwareSerial& ibusRcSerial = Serial1; // Uses RX1 (Pin 19) on Arduino Mega

// Define easy-to-read names for your 3-way switch positions
enum Switch3Way {SWITCH_UP,SWITCH_MID,SWITCH_DOWN};

//Function Prototypes
int readChannel(byte channelInput, int minLimit, int maxLimit, int defaultValue);
bool readSwitch(byte channelInput, bool defaultValue);
Switch3Way read3WaySwitch(byte channelInput, Switch3Way defaultValue);

void setup() {
  debugSerial.begin(115200);           // Debug serial
  ibusRc.begin(ibusRcSerial);     // Start iBus on Serial1
  debugSerial.println("iBus Receiver Control Subsystem Initialized.");
}

void loop() {
  // Check if the transmitter is turned off or disconnected
  // The library returns 0 on channel 0 if no valid RC packets are coming in
  if (ibusRc.readChannel(0) == 0) {
    debugSerial.println("WARNING: Signal Lost / Transmitter Disconnected!");
    delay(500);
    return; // Skip reading channels if link is dead
    }

  // Read and print all 10 channels
  for (int i = 0; i < 10; i++) {
    int value = readChannel(i, -100, 100, 0);
    debugSerial.print("Ch");
    debugSerial.print(i + 1);
    debugSerial.print(": ");
    debugSerial.print(value);
    debugSerial.print("\t");
  }
  debugSerial.println();

  delay(20);  // 20ms matches the typical iBUS frame transmission rate
}


// Read the channel and convert to the range provided, constrained safely
int readChannel(byte channelInput, int minLimit, int maxLimit, int defaultValue){
  uint16_t ch = ibusRc.readChannel(channelInput);

  // If the library returns 0, it means it hasn't received data for this channel yet
  if (ch == 0) return defaultValue;

  // Map the value and constrain it so it never goes past minLimit or maxLimit
  int mappedValue = map(ch, 1000, 2000, minLimit, maxLimit);
  return constrain(mappedValue, minLimit, maxLimit);
}

// Read a standard 2-way switch and return a boolean value
bool readSwitch(byte channelInput, bool defaultValue){
  int intDefaultValue = (defaultValue)? 100: 0;
  int ch = readChannel(channelInput, 0, 100, intDefaultValue);
  return (ch > 50);
}

// Read a 3-way switch (like SwC) and return UP, MID, or DOWN state
Switch3Way read3WaySwitch(byte channelInput, Switch3Way defaultValue) {
  uint16_t ch = ibusRc.readChannel(channelInput);

  // If the library returns 0, use the chosen default position
  if (ch == 0) return defaultValue;

  // Evaluate based on the FS-i6X physical signal ranges
  if (ch > 1750) {
    return SWITCH_UP;     // Raw pulse width sits around 2000
    }
    else if (ch >= 1250 && ch <= 1750) {
      return SWITCH_MID;    // Raw pulse width sits around 1500
      }
      else {
        return SWITCH_DOWN;   // Raw pulse width sits around 1000
        }
}

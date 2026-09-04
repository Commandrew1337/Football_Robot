#include <Arduino.h>
#include "RcController.cpp"

// Instantiate the custom RC controller object globally by passing your hardware serial objects
RcController controller(Serial1, Serial2, Serial);

void setup() {
  Serial.begin(115200); // Debug serial to PC
  controller.begin();   // Initialize all internal radio subsystems
}

void loop() {
  // 1. RUN CONSTANTLY: Processes radio signals at full microprocessor speed.
  // Never place a blocking delay() anywhere in this loop!
  controller.update(); 

  // 2. SAFETY FAIL-SAFE: If connection drops, stop here before moving motors
  if (!controller.isConnected()) {
    // ==> PUT EMERGENCY STOP CODE HERE (e.g., turn off motor pins) <==
    return; 
  }

  // 3. THE 20ms HEARTBEAT GATE: 
  // All motor speeds, steering positions, and telemetry must be updated inside here!
  if (controller.isReadyToProcess()) {
    
    // A. Gather your driving inputs (Perfectly synchronized with the 20ms radio transmission)
    int throttle = controller.readChannel(2, -255, 255, 0); // Ch3: Typical Throttle Stick
    int steering = controller.readChannel(0, -255, 255, 0); // Ch1: Typical Roll/Steer Stick
    
    // B. ==> PUT MOTOR DRIVER UPDATES HERE <==
    // This runs exactly 50 times per second, giving motor chips a clean, steady control signal.
    // Examples:
    // analogWrite(leftMotorPin, throttle); 
    // steeringServo.write(steering);

    // C. Send live data back to FlySky Transmitter screen (Capped to 20ms to avoid flooding buffers)
    int liveBatteryVolt = 1240; // 12.40V
    controller.sendBatteryVoltage(liveBatteryVolt);

    // D. Safe to call debug printing. The object will throttle itself internally 
    // to print slowly, keeping your driving experience perfectly smooth.
    controller.printDebugChannels(); 
  }
}
#include <Arduino.h>

#include "robotConfig.h"
#include "RcController.h"
#include "PWMMotorController.h"
#include "RSL.h"
#include "RelayValve.h"

#include "RobotDrive.h"

RcController FScontroller(Serial1, Serial2, Serial);
RelayValve singlerelay1(robotConfig::RELAY_IN3, true);
RelayValve singlerelay2(robotConfig::RELAY_IN4, true);
RelayValve doublerelay3(robotConfig::RELAY_IN1, robotConfig::RELAY_IN2, 100, true);

void setup() {
  Serial.begin(MON_BAUD_RATE);
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
    //int throttle = FScontroller.readChannel(2, -255, 255, 0);
    //int steering = FScontroller.readChannel(0, -255, 255, 0);

    // ==> EXECUTE DRIVING OUTPUT SCHEDULERS HERE <==

    // Feed current system voltage reading back to FScontroller screen
    int liveBatteryVolt = 1240; 
    FScontroller.sendBatteryVoltage(liveBatteryVolt);

    FScontroller.readSwitch(robotConfig::CH_SWA, false) ? singlerelay1.activate() : singlerelay1.deactivate();
    FScontroller.readSwitch(robotConfig::CH_SWB, false) ? singlerelay2.activate() : singlerelay2.deactivate();
    FScontroller.readSwitch(robotConfig::CH_SWD, false) ? doublerelay3.activate() : doublerelay3.deactivate();

    // Call diagnostic tool safely without introducing motor stuttering lags
    FScontroller.printDebugChannels(); 
  }
}

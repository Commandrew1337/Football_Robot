#include <Arduino.h>

#include "robotConfig.h"
#include "RcController.h"
#include "PWMMotorController.h"
#include "RSL.h"
#include "RelayValve.h"
#include "Compressor.h"

#include "RobotDrive.h"
#include "BatteryMonitor.h"

RcController FScontroller(Serial1, Serial2, Serial);
RelayValve singlerelay1(robotConfig::RELAY_IN3, true);
RelayValve singlerelay2(robotConfig::RELAY_IN4, true);
RelayValve doublerelay3(robotConfig::RELAY_IN1, robotConfig::RELAY_IN2, 100, true);
Compressor m_compressor(robotConfig::COMPRESSOR_PRESSURE_SWITCH, robotConfig::COMPRESSOR_SPIKE_FORWARD, robotConfig::COMPRESSOR_SPIKE_REVERSE);
RSL m_RSL(robotConfig::RSL_PIN);
PWMMotorController leftFront(robotConfig::MLF,PWMMotorController::ControllerType::Talon);
PWMMotorController leftRear(robotConfig::MLR,PWMMotorController::ControllerType::Talon);
PWMMotorController rightFront(robotConfig::MRF,PWMMotorController::ControllerType::Talon,true);
PWMMotorController rightRear(robotConfig::MRR,PWMMotorController::ControllerType::Talon,true);

RobotDrive drive(leftFront, leftRear, rightFront, rightRear, 0.05); // 5% joystick deadband
BatteryMonitor battery(robotConfig::LIVE_BATT, robotConfig::R1, robotConfig::R2, robotConfig::ARDUINO_VCC, 500);



unsigned long lastSafetyCheckTime = 0;

void setup() {
  Serial.begin(MON_BAUD_RATE);
  FScontroller.begin();
  leftFront.begin();
  leftRear.begin();
  rightFront.begin();
  rightRear.begin();
  battery.begin();
}

void loop() {
  FScontroller.update(); // Keep background telemetry and serial caching alive
  battery.update();

  bool robotEnabled = FScontroller.isReceiverHardwareConnected();
  m_RSL.setEnabled(robotEnabled);

  //m_compressor.update();
  m_RSL.update();

  // HARDWARE SAFEGUARD: Executes if the controller is off or disconnected
  if (!robotEnabled) {
    unsigned long currentTime = millis();
    
    // Slow down the safety loop to execute only once every 200 milliseconds (5Hz)
    if (currentTime - lastSafetyCheckTime >= 200) {
      lastSafetyCheckTime = currentTime;
      
      singlerelay1.deactivate();
      singlerelay2.deactivate();
      doublerelay3.deactivate(); 
      drive.stop();
    }
    return; // Safe to return here because the timer throttles the code paths below
  }

  // Execute operations within the steady 20ms frame interval
  if (FScontroller.isReadyToProcess()) {
    
    // Read and interpret stick movements smoothly.
    // If transmitter is off, these will automatically return whatever default failsafe 
    // values you configured directly inside your FlySky Transmitter setup menu.

    // ==> EXECUTE DRIVING OUTPUT SCHEDULERS HERE <==

    // Feed current system voltage reading back to FScontroller screen
    FScontroller.sendBatteryVoltage(battery.getTelemetryVoltage());

    FScontroller.readSwitch(robotConfig::CH_SWA, false) ? singlerelay1.activate() : singlerelay1.deactivate();
    FScontroller.readSwitch(robotConfig::CH_SWB, false) ? singlerelay2.activate() : singlerelay2.deactivate();
    FScontroller.readSwitch(robotConfig::CH_SWD, false) ? doublerelay3.activate() : doublerelay3.deactivate();

    drive.drive(FScontroller, robotConfig::CH_PITCH, robotConfig::CH_ROLL);

    

    // Call diagnostic tool safely without introducing motor stuttering lags
    //FScontroller.printDebugChannels(); 
  }
}

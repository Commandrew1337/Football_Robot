#include <Arduino.h>

#include "robotConfig.h"
#include "RcController.h"
#include "PWMMotorController.h"
#include "RSL.h"
#include "RelayValve.h"
#include "Compressor.h"
#include "SpikeRelay.h"
#include "LaunchWheels.h"

#include "RobotDrive.h"
#include "BatteryMonitor.h"

RcController FScontroller(Serial1, Serial2, Serial);
RelayValve singlerelay1(robotConfig::RELAY_IN3, true);
RelayValve singlerelay2(robotConfig::RELAY_IN4, true);
RelayValve doublerelay3(robotConfig::RELAY_IN1, robotConfig::RELAY_IN2, 100, true);
Compressor m_compressor(robotConfig::COMPRESSOR_PRESSURE_SWITCH, robotConfig::COMPRESSOR_SPIKE_FORWARD, robotConfig::COMPRESSOR_SPIKE_REVERSE);
RSL m_RSL(robotConfig::RSL_PIN);
SpikeRelay horn(robotConfig::HORN_SPIKE_FORWARD,robotConfig::HORN_SPIKE_REVERSE);
SpikeRelay lights(robotConfig::LIGHTS_SPIKE_FORWARD,robotConfig::LIGHTS_SPIKE_REVERSE);
PWMMotorController leftFront(robotConfig::MLF,PWMMotorController::ControllerType::Talon);
PWMMotorController leftRear(robotConfig::MLR,PWMMotorController::ControllerType::Talon);
PWMMotorController rightFront(robotConfig::MRF,PWMMotorController::ControllerType::Talon,true);
PWMMotorController rightRear(robotConfig::MRR,PWMMotorController::ControllerType::Talon,true);

PWMMotorController launcherLeftA(robotConfig::MT2,PWMMotorController::ControllerType::Talon);
PWMMotorController launcherLeftB(robotConfig::MT4,PWMMotorController::ControllerType::Talon,true);
PWMMotorController launcherRightA(robotConfig::MT1,PWMMotorController::ControllerType::Talon);
PWMMotorController launcherRightB(robotConfig::MT3,PWMMotorController::ControllerType::Talon,true);
LaunchWheels launchWheels(launcherLeftA,launcherLeftB,launcherRightA,launcherRightB);

RobotDrive drive(leftFront, leftRear, rightFront, rightRear, 0.05); // 5% joystick deadband
BatteryMonitor battery(robotConfig::LIVE_BATT, robotConfig::R1, robotConfig::R2, robotConfig::ARDUINO_VCC, 1000);

unsigned long lastSafetyCheckTime = 0;
unsigned long lastTelemetryTime = 0; // Added tracking metric for isolated telemetry frames

void setup() {
  Serial.begin(MON_BAUD_RATE);
  FScontroller.begin();
  leftFront.begin();
  leftRear.begin();
  rightFront.begin();
  rightRear.begin();
  battery.begin();
  launcherLeftA.begin();
  launcherLeftB.begin();
  launcherRightA.begin();
  launcherRightB.begin();
  launchWheels.enable();
  launchWheels.stop();
  horn.off();
  lights.off();
  m_compressor.stop();
}

void loop() {
  FScontroller.update(); // Keep background telemetry and serial caching alive
  
  unsigned long currentTime = millis();
  
  // Throttles background telemetry transmission frames to a slow 5Hz rate.
  // This relieves high-frequency loop noise and Serial2 half-duplex back-feeding 
  // issues from echoing directly into your logic pins while the robot sits idle.
  if (currentTime - lastTelemetryTime >= 200) {
    lastTelemetryTime = currentTime;
    battery.update();
    FScontroller.sendBatteryVoltage(battery.getTelemetryVoltage());
  }

  bool robotEnabled = FScontroller.isReceiverHardwareConnected();
  m_RSL.setEnabled(robotEnabled);

  m_compressor.update();
  m_RSL.update();

  // HARDWARE SAFEGUARD: Executes if the controller is off or disconnected
  if (!robotEnabled) {
    // Slow down the safety loop to execute only once every 200 milliseconds (5Hz)
    if (currentTime - lastSafetyCheckTime >= 200) {
      lastSafetyCheckTime = currentTime;
      
      singlerelay1.deactivate();
      singlerelay2.deactivate();
      doublerelay3.deactivate(); 
      drive.stop();
      horn.off();
      lights.off();
    }
    return; // Safe to return here because the timer throttles the code paths below
  }

  // Execute operations within the steady 20ms frame interval
  if (FScontroller.isReadyToProcess()) {
    
    // Read and interpret stick movements smoothly.
    // If transmitter is off, these will automatically return whatever default failsafe 
    // values you configured directly inside your FlySky Transmitter setup menu.

    // ==> EXECUTE DRIVING OUTPUT SCHEDULERS HERE <==
    drive.drive(FScontroller, robotConfig::CH_PITCH, robotConfig::CH_ROLL);

    FScontroller.readSwitch(robotConfig::CH_SWA, false) ? singlerelay1.activate() : singlerelay1.deactivate();
    FScontroller.readSwitch(robotConfig::CH_SWB, false) ? singlerelay2.activate() : singlerelay2.deactivate();
    FScontroller.readSwitch(robotConfig::CH_SWD, false) ? lights.forward() : lights.off();

    int FSyaw = FScontroller.readChannel(robotConfig::CH_YAW,-100,100,0);
    if (abs(FSyaw)>95){
      horn.forward();
    } else {
      horn.off();
    }

    Switch3Way SWCPos = FScontroller.read3WaySwitch(robotConfig::CH_SWC,SWITCH_UP);
    switch (SWCPos) {
      case Switch3Way::SWITCH_UP:
          m_compressor.disable();
          break;
      case Switch3Way::SWITCH_MID:
          m_compressor.enable();
          break;
      case Switch3Way::SWITCH_DOWN:
          m_compressor.enable();
          break;
    }

    if (launchWheels.isEnabled()) {
        launchWheels.launch(FScontroller,robotConfig::CH_THROTTLE);
    }
  }

    // Call diagnostic tool safely without introducing motor stuttering lags
    //FScontroller.printDebugChannels(); 
}
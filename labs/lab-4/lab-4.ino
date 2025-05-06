#include <Servo.h>
#include "command.h"
#include "sensors.h"
#include "PID.h"
#include "constants.h"

volatile bool pidFlag = false;
volatile bool distancePidFlag = false;
volatile bool speedFlag = false;
volatile bool sendFeedbackFlag = false;
volatile bool triggerNextProximityFlag = false;
volatile float targetSpeed = 0;
volatile float targetMaxSpeed = 0; 
volatile float targetCenterOffset = 0;
volatile float motorActuation = 0;
volatile float steeringActuation = 0;
volatile bool motorStarted = false;

PIDData motorPID;
PIDData distancePID;
ProximitySensor rightProximitySensor, leftProximitySensor, forwardProximitySensor;
TriggerState triggerState = TRIGGER_RIGHT;

void setup() {
  setupSensorData();
  setupSerialUSB(BAUD_RATE);
  setupSerialRock(BAUD_RATE);
  setupTimerInterruptChannel0(SERIAL_FEEDBACK_FREQUENCY);
  setupTimerInterruptChannel1(PID_SAMPLING_FREQUENCY);
  setupTimerInterruptChannel2(SPEED_SENSOR_UPDATE_FREQUENCY);
  setupSteering(STEERING_PIN);
  setupMotor(MOTOR_PIN);
  setupSpeedSensor(SPEED_SENSOR_PIN);
  setupPID(&motorPID, (1000.0f*(1.0f/((float)PID_SAMPLING_FREQUENCY))),MAX_ACCUM_ERROR, KP_SPEED, KI_SPEED, KD_SPEED);
  setupProximitySensor(RIGHT_PROXIMITY_ECHO_PIN, RIGHT_PROXIMITY_TRIGGER_PIN, rightProximitySensor, rightProximityISR);
  setupProximitySensor(LEFT_PROXIMITY_ECHO_PIN, LEFT_PROXIMITY_TRIGGER_PIN, leftProximitySensor, leftProximityISR);
  setupProximitySensor(FORWARD_PROXIMITY_ECHO_PIN, FORWARD_PROXIMITY_TRIGGER_PIN, forwardProximitySensor, forwardProximityISR);
  setupPID(&distancePID, (1000.0f*(1.0f/((float)PID_SAMPLING_FREQUENCY))),MAX_ACCUM_ERROR, KP_DIST, KI_DIST, KD_DIST);
  DEBUG_PRINTLN("Setup complete!");
}

// Interrupt handler for timer interrupt channel 0
void TC6_Handler() {
  TC_GetStatus(TC2, 0);           // Clears interrupt flag
  sendFeedbackFlag = true;
  triggerNextProximityFlag = true;
}

// Interrupt handler for timer interrupt channel 1
void TC7_Handler() {
  TC_GetStatus(TC2, 1);           // Clears interrupt flag
  pidFlag = true;
  distancePidFlag = true;
}

// Interrupt handler for timer interrupt channel 2
void TC8_Handler() {
  TC_GetStatus(TC2, 2);           // Clears interrupt flag
  speedFlag = true;
}

// Flag-based programming needed for Arduino framework
void loop() {
    if (speedFlag) {
      calcCurrSpeed();
      speedFlag = false;
    }
    if (pidFlag && motorStarted) {
      float distanceToFrontWall = getProximityRange(forwardProximitySensor);
      targetSpeed = speedLimiter(targetMaxSpeed, distanceToFrontWall);
      motorActuation = PIDControl(&motorPID, getSpeed(), targetSpeed);
      setTargetMotorRPMPercent(motorActuation);
      pidFlag = false;
    }
    if (distancePidFlag && motorStarted) {
      // Must check if positive offset = right or left
      float currentCenterOffset = getProximityRange(rightProximitySensor) - getProximityRange(leftProximitySensor);
      steeringActuation = PIDControl(&distancePID, currentCenterOffset, targetCenterOffset);
      changeSteeringAngle(steeringActuation);
      distancePidFlag = false;
    }
    if (sendFeedbackFlag) {
      sendFeedback();
      sendFeedbackFlag = false;
    }  
    if (triggerNextProximityFlag) {
      triggerNextProximityFlag = !triggerProximitySensor(); // Returns true if the trigger is completed
    }

    readSerial(Serial1);
    if DEBUG {
        readSerial(Serial);
    }
}


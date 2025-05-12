#include <Servo.h>
#include "command.h"
#include "sensors.h"
#include "PID.h"
#include "constants.h"
#include "state.h"
#include "slidingMode.h"

volatile bool speedPidFlag = false;
volatile bool distancePidFlag = false;
volatile bool speedSensorFlag = false;
volatile bool sendFeedbackFlag = false;
volatile bool triggerNextProximityFlag = false;
volatile float targetSpeed = 0;
volatile float motorActuation = 0;
volatile float steeringActuation = 0;
volatile bool motorStarted = false;

PIDData motorPID;
PIDData distancePID;
ProximitySensor rightProximitySensor, leftProximitySensor, forwardProximitySensor;
TriggerState triggerState = TRIGGER_RIGHT;
state car;
slidingMode controller;

void setup() {
  setupSensorData();
  setupSerialUSB(BAUD_RATE);
  setupSerialRock(BAUD_RATE);
  setupTimerInterruptChannel0(SERIAL_FEEDBACK_FREQUENCY);
  setupTimerInterruptChannel1(PID_SAMPLING_FREQUENCY);
  setupTimerInterruptChannel2(PROXIMITY_SENSOR_UPDATE_FREQUENCY);
  setupSteering(STEERING_PIN);
  setupMotor(MOTOR_PIN);
  setupSpeedSensor(SPEED_SENSOR_PIN);
  setupPID(&motorPID, (1000.0f*(1.0f/((float)PID_SAMPLING_FREQUENCY))),MAX_ACCUM_ERROR, KP_SPEED, KI_SPEED, KD_SPEED);
  setupProximitySensor(RIGHT_PROXIMITY_ECHO_PIN, RIGHT_PROXIMITY_TRIGGER_PIN, rightProximitySensor, rightProximityISR);
  setupProximitySensor(LEFT_PROXIMITY_ECHO_PIN, LEFT_PROXIMITY_TRIGGER_PIN, leftProximitySensor, leftProximityISR);
  setupProximitySensor(FORWARD_PROXIMITY_ECHO_PIN, FORWARD_PROXIMITY_TRIGGER_PIN, forwardProximitySensor, forwardProximityISR);
  setupPID(&distancePID, (1000.0f*(1.0f/((float)PID_SAMPLING_FREQUENCY))),MAX_ACCUM_ERROR, KP_DIST, KI_DIST, KD_DIST);
  setupSlidingMode(&controller, ((1.0f/((float)PID_SAMPLING_FREQUENCY))), 0.01, 0.5);
  setupState(&car);
  DEBUG_PRINTLN("Setup complete!");
}

// Interrupt handler for timer interrupt channel 0
void TC6_Handler() {
  TC_GetStatus(TC2, 0);           // Clears interrupt flag
  sendFeedbackFlag = true;
  speedSensorFlag = true;
}

// Interrupt handler for timer interrupt channel 1
void TC7_Handler() {
  TC_GetStatus(TC2, 1);           // Clears interrupt flag
  speedPidFlag = true;
  distancePidFlag = true;
}

// Interrupt handler for timer interrupt channel 2
void TC8_Handler() {
  TC_GetStatus(TC2, 2);           // Clears interrupt flag
    triggerNextProximityFlag = true;
}

// Flag-based programming needed for Arduino framework
void loop() {
    if (speedSensorFlag) {
      calcCurrSpeed();
      speedSensorFlag = false;
    }
    if (speedPidFlag && motorStarted) {
      float distanceToFrontWall = getProximityRange(forwardProximitySensor);
      float speed = (distanceToFrontWall>MIN_DISTANCE_TO_FRONT_WALL_CM) ? targetSpeed : 0.0;
      motorActuation = PIDControl(&motorPID, getSpeed(), speed);
      setTargetMotorRPMPercent(motorActuation);
      car.speed = getSpeed();
      speedPidFlag = false;
    }
    if (distancePidFlag && motorStarted) {
      // Must check if positive offset = right or left
      float b = getProximityRange(rightProximitySensor);
      float d = getProximityRange(leftProximitySensor);
      measurement(&car, d, b);
      slidingModeControl(&car, &controller);  // PIDControl(&distancePID, currentCenterOffset, targetCenterOffset);
      changeSteeringAngle(car.alpha*180/PI);  // konverterar från radianer till grader
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


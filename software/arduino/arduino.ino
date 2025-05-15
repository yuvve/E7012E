#include <Servo.h>
#include "command.h"
#include "sensors.h"
#include "PID.h"
#include "constants.h"

volatile bool sendFeedbackFlag = false;

volatile bool distancePidFlag = false;
volatile bool turningRoutine = false;
volatile uint turningTimerMillis = 0;
volatile float steeringActuation = 0;

volatile bool speedSensorFlag = false;
volatile bool triggerNextProximityFlag = false;

volatile float targetSpeed = 0.0;
volatile float targetSpeedOrStop = 0.0;
volatile float motorActuation = 0.0;
volatile bool motorStarted = false;
volatile bool steeringStarted = false;

volatile float kpSlow = KP_SLOW;
volatile float kiSlow = KI_SLOW;
volatile float kdSlow = KD_SLOW;
volatile float kpFast = KP_FAST;
volatile float kiFast = KI_FAST;
volatile float kdFast = KD_FAST;
volatile float frontDistanceToStartTurning = FRONT_DISTANCE_TO_START_TURNING;
volatile float speedPercentFast = SPEED_PERCENT_FAST;
volatile float speedPercentSlow = SPEED_PERCENT_SLOW;
volatile uint turningRoutineTimerMS = TURNING_ROUTINE_DELAY_OFF_TIMER_MS;
volatile float minDistanceToFrontWallCm = MIN_DISTANCE_TO_FRONT_WALL_CM;

PIDData distancePID;
ProximitySensor rightProximitySensor, leftProximitySensor, forwardProximitySensor;
TriggerState triggerState = TRIGGER_RIGHT;

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
  setupProximitySensor(RIGHT_PROXIMITY_ECHO_PIN, RIGHT_PROXIMITY_TRIGGER_PIN, rightProximitySensor, rightProximityISR);
  setupProximitySensor(LEFT_PROXIMITY_ECHO_PIN, LEFT_PROXIMITY_TRIGGER_PIN, leftProximitySensor, leftProximityISR);
  setupProximitySensor(FORWARD_PROXIMITY_ECHO_PIN, FORWARD_PROXIMITY_TRIGGER_PIN, forwardProximitySensor, forwardProximityISR);
  setupPID(&distancePID, (1000.0f*(1.0f/((float)PID_SAMPLING_FREQUENCY))),MAX_ACCUM_ERROR, KP_SLOW, KI_SLOW, KD_SLOW);
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
  distancePidFlag = true;
}

// Interrupt handler for timer interrupt channel 2
void TC8_Handler() {
  TC_GetStatus(TC2, 2);           // Clears interrupt flag
    triggerNextProximityFlag = true;
}

void goFast(){
    noInterrupts();
    turningRoutine = false;
    adjustPID(&distancePID, kpFast, kiFast, kdFast);
    targetSpeed = speedPercentFast;
    interrupts();
    DEBUG_PRINTLN("Fast mode started!");
}

void goSlow(){
    noInterrupts();
    turningRoutine = true;
    turningTimerMillis = millis();
    targetSpeed = speedPercentSlow;
    adjustPID(&distancePID, kpSlow, kiSlow, kdSlow);
    interrupts();
    DEBUG_PRINTLN("Slow mode started!");
}

// Flag-based programming needed for Arduino framework
void loop() {
    if (speedSensorFlag) {
      calcCurrSpeed();
      speedSensorFlag = false;
    }
    if (motorStarted) {
      float distanceToFrontWall = getProximityRange(forwardProximitySensor);
      targetSpeedOrStop = (distanceToFrontWall>minDistanceToFrontWallCm) ? targetSpeed : 0.0;
      setTargetMotorRPMPercent(targetSpeedOrStop);
    }
    if (distancePidFlag && steeringStarted) {
      float currentCenterOffset = getProximityRange(rightProximitySensor) - getProximityRange(leftProximitySensor);
      float distanceToFrontWall = getProximityRange(forwardProximitySensor);

      if (distanceToFrontWall <= frontDistanceToStartTurning) {
        if (turningRoutine) {
          turningTimerMillis = millis();
        } else {
          goSlow();
        }
      } else {
        if ((turningRoutine) && ((millis() - turningTimerMillis) >= turningRoutineTimerMS)) {
          goFast();
        }
      }

      steeringActuation = PIDControl(&distancePID, currentCenterOffset, 0);
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


#include <Servo.h>
#include "command.h"
#include "sensors.h"
#include "PID.h"
#include "constants.h"

volatile bool sendFeedbackFlag = false;
volatile bool pidFlag = false;
volatile bool speedFlag = false;
volatile bool motorStarted = false;
volatile float targetSpeed = 0; 
volatile float targetAngle = 0;
volatile float motorTargetRPMPercent = 0;

PIDData motorPID;
PIDData distancePID;

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
    setupPID(&motorPID, (1000.0f*(1.0f/((float)PID_SAMPLING_FREQUENCY))),MAX_ACCUM_ERROR, START_KP, START_KI, START_KD);
    if DEBUG {
    Serial.println("Setup complete!");
  }
}

// Interrupt handler for timer interrupt channel 0
void TC6_Handler() {
  TC_GetStatus(TC2, 0);           // Clears interrupt flag
  sendFeedbackFlag = true;
}

// Interrupt handler for timer interrupt channel 1
void TC7_Handler() {
  TC_GetStatus(TC2, 1);           // Clears interrupt flag
  pidFlag = true;
}

// Interrupt handler for timer interrupt channel 2
void TC8_Handler() {
  TC_GetStatus(TC2, 2);           // Clears interrupt flag
  speedFlag = true;
}

void processSerialInput(const char* input) {
  Command cmd = parseSerialInput(input);
  switch (cmd.cmd) {
    case 'M':
      targetSpeed = cmd.value;
      if (targetSpeed == 0){
        motorStarted = false; 
        setTargetMotorRPMPercent(0);
        resetPID(&motorPID);
      } else {
        motorStarted = true;
      }
      break;
    case 'S':
      targetAngle = cmd.value;
      changeSteeringAngle(cmd.value);
      break;
    case 'P':
      adjustP(&motorPID, cmd.value);
      break;
    case 'I':
      adjustI(&motorPID, cmd.value);
      break;
    case 'D':
      adjustD(&motorPID, cmd.value);
      break;
    default:
      break;
  }
}

// Flag-based programming needed for Arduino framework
void loop() {
    if (speedFlag) {
      calcCurrSpeed();
      speedFlag = false;
    }
    if (pidFlag && motorStarted) {
      motorTargetRPMPercent = PIDControl(&motorPID, getSpeed(), targetSpeed);
      setTargetMotorRPMPercent(motorTargetRPMPercent);
      pidFlag = false;
    }
    if (sendFeedbackFlag) {
      sendFeedback();
      sendFeedbackFlag = false;
    }
    readSerial(Serial1);
    if DEBUG {
        readSerial(Serial);
    }
}


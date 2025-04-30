#include <Servo.h>
#include "command.h"
#include "sensors.h"
#include "PID.h"
#include "constants.h"

volatile bool sendFeedbackFlag = false;
volatile bool pidFlag = false;
volatile bool speedFlag = false;
volatile bool proximityEchoFlag = false;
volatile bool proximityTriggerFlag = false;
volatile float targetSpeed = 0; 
volatile float targetAngle = 0;
volatile float motorTargetRPMPercent = 0;
volatile bool motorStarted = false;

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
  setupProximitySensor(PROXIMITY_TRIGGER_PIN, PROXIMITY_ECHO_PIN);
  DEBUG_PRINTLN("Setup complete!");
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
    if (proximityEchoFlag) {
      proximityEchoFlag = false;
      calculateProximityRange();
    }

    sendProximityPulse();

    readSerial(Serial1);
    if DEBUG {
        readSerial(Serial);
    }
}


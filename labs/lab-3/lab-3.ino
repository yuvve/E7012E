#include <Servo.h>
#include "command.h"
#include "sensors.h"
#include "PID.h"

#define DEBUG (1)
#define WHEEL_DIAM_CM 6.5
#define SERIAL_FEEDBACK_FREQUENCY 1
#define PID_SAMPLING_FREQUENCY 25
#define SPEED_SENSOR_PIN 54
#define MOTOR_PIN 9
#define STEERING_PIN 10
#define BAUD_RATE 115200

volatile bool sendFeedbackFlag = false;
volatile bool pidFlag = false;
volatile float targetSpeed = 0; 
volatile float targetAngle = 0;
PIDData motorPID;
PIDData distancePID;

void setup() {
    setupSensorData();
    setupSerialUSB(BAUD_RATE);
    setupSerialRock(BAUD_RATE);
    setupTimerInterruptChannel0(SERIAL_FEEDBACK_FREQUENCY);
    setupTimerInterruptChannel1(PID_SAMPLING_FREQUENCY);
    setupSteering(STEERING_PIN);
    setupMotor(MOTOR_PIN);
    setupSpeedSensor(SPEED_SENSOR_PIN);
    setupPID(&motorPID, (1000.0f*(1.0f/((float)PID_SAMPLING_FREQUENCY))), 1.0, 1.0, 1.0);
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
}

// Flag-based programming needed for Arduino framework
void loop() {
    if (pidFlag) {
      setTargetMotorRPMPercent(PIDControl(&motorPID, getSpeed(), targetSpeed));
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


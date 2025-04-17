#include <Servo.h>
#include "command.h"
#include "sensors.h"
#include "PID.h"

#define DEBUG (1)
#define WHEEL_DIAM_CM 6.5
#define SAMPLING_PERIOD_MS 100
#define SERIAL_FEEDBACK_FREQUENCY 1
#define PID_SAMPLING_FREQUENCY 1/(SAMPLING_PERIOD_MS*1000)

volatile float targetSpeed = 0; 
volatile float targetAngle = 0;
PIDData motorPID;
PIDData distancePID;

void setup() {
    setupSensorData();
    setupSerialUSB(9600);
    setupSerialRock(115200);
    setupTimer();
    setupTimerInterruptChannel0(SERIAL_FEEDBACK_FREQUENCY);
    setupTimerInterruptChannel1(PID_SAMPLING_FREQUENCY);
    setupSteering(10);
    setupMotor(9);
    setupSpeedSensor(54);
    setupPID(&motorPID, SAMPLING_PERIOD_MS, 1.0, 1.0, 1.0);
}

// Interrupt handler for timer interrupt channel 0
void TC6_Handler() {
  TC_GetStatus(TC2, 0);           // Clears interrupt flag
  sendFeedback();
}

// Interrupt handler for timer interrupt channel 1
void TC7_Handler() {
  TC_GetStatus(TC2, 1);           // Clears interrupt flag
  setTargetMotorRPMPercent(PIDControl(&motorPID, getSpeed(), targetSpeed));
}

// Interrupt handler for timer interrupt channel 2
void TC8_Handler() {
  TC_GetStatus(TC2, 2);           // Clears interrupt flag
}

void loop() {
    processSerial(Serial1);
    if DEBUG {
        processSerial(Serial);
    }
}


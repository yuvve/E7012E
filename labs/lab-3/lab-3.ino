#include <Servo.h>
#include "command.h"
#include "sensors.h"
#include "PID.h"

#define DEBUG (1)
#define WHEEL_DIAM_CM 6.5
#define SAMPLING_PERIOD_MS 100

volatile float timeSinceLastPIDUpdate = 0;
volatile float lastTime = 0; 
volatile float targetSpeed = 0; 
volatile float targetAngle = 0;
volatile bool sendFeedbackFlag = false;

PIDData motorPID;
PIDData distancePID;

void setup() {
    setupSensorData();
    setupSerialUSB(9600);
    setupSerialRock(115200);
    setupFeedbackTimer(1);
    setupSteering(10);
    setupMotor(9);
    setupSpeedSensor(54);
    setupPID(&motorPID, SAMPLING_PERIOD_MS, 1.0, 1.0, 1.0);
}

void loop() {
    float currTime = millis();
    float deltaTime =  currTime - lastTime;
    lastTime = currTime;
    timeSinceLastPIDUpdate += deltaTime;

    if (timeSinceLastPIDUpdate >= SAMPLING_PERIOD_MS) {
       setTargetMotorRPMPercent(PIDControl(&motorPID, getSpeed(), targetSpeed));
       timeSinceLastPIDUpdate = 0;
    }

    if (sendFeedbackFlag) {
        sendFeedbackFlag = 0;
        sendFeedback();
    }

    processSerial(Serial1);
    if DEBUG {
        processSerial(Serial);
    }
}


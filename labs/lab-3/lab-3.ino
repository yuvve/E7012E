#include <Servo.h>
#include "command.h"
#include "sensors.h"
#include "PID.h"

#define DEBUG (1)
#define WHEEL_RAD_CM 6.5
#define SAMPLING_PERIOD_MS 100 

volatile float timeSinceLastPIDUpdate = 0;
volatile float lastTime = 0; 
volatile float targetSpeed = 0; 

void setup() {
    setupSensorData();
    setupSerial();
    setupSteering(10);
    setupMotor(9);
    setupSpeedSensor(54);
    setupPID(SAMPLING_PERIOD_MS, 1.0, 1.0, 1.0);
}

void loop() {
    float currTime = millis();
    float deltaTime = lastTime - currTime;
    lastTime = currTime;
    timeSinceLastPIDUpdate += deltaTime;

    if (timeSinceLastPIDUpdate >= SAMPLING_PERIOD_MS) {
       setTargetMotorRPMPercent(PIDControl(getSpeed(), target_speed));
    }

    if (Serial.available()) {
        String input = Serial.readStringUntil('\n');
        Command cmd = parseSerialInput(input);
        switch (cmd.cmd) {
            case 'M':
                target_speed = cmd.value;
                break;
            case 'S':
                changeSteeringAngle(cmd.value);
                break;
            default: 
                break;
        }
    }
}


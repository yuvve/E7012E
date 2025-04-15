#include <Servo.h>
#include "command.h"
#include "sensors.h"
#include "PID.h"
#define DEBUG (1)
#define WHEEL_RAD 1.0

void setup() {
    setupSensorData();
    setupSerial();
    setupSteering(10);
    setupMotor(9);
    setupSpeedSensor(54);
    setupPID(1.0, 1.0, 1.0);
}

void loop() {
    if (Serial.available()) {
        String input = Serial.readStringUntil('\n');
        Command cmd = parseSerialInput(input);
        switch (cmd.cmd) {
            case 'M':
                setTargetMotorRPMPercent(PIDControl(getSpeed(), cmd.value));
                break;
            case 'S':
                changeSteeringAngle(cmd.value);
                break;
            default: 
                break;
        }
    }
}


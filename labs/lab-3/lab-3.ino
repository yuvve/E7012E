#include <Servo.h>
#include "command.h"
#include "sensor_data.h"
#define DEBUG (1)
#define WHEEL_RAD 1.0;

Servo steering;
Servo motor;
SensorData sensorData;

void setup() {
    sensorData = SensorData {0.0};
    setupSerial();
    setupSteering(&steering, 10);
    setupMotor(&motor, 9);
    setupSpeedSensor(54, &sensorData);
}

void loop() {
    if (Serial.available()) {
        String input = Serial.readStringUntil('\n');
        Command cmd = parseSerialInput(input);
        switch (cmd.cmd) {
            case 'M':
                setTargetMotorRPMPercent(&motor, cmd.value);
                break;
            case 'S':
                changeAngle(&steering, cmd.value);
                break;
            default: 
                break;
        }
    }
}


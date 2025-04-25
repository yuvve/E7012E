#ifndef CONSTANTS_H
#define CONSTANTS_H
  // Global settings
  #define DEBUG (1)

  // Serial settings
  #define SERIAL_BUFFER_SIZE 32
  #define SERIAL_FEEDBACK_FREQUENCY 10
  #define BAUD_RATE 115200

  // Speed sensor settings
  #define SPEED_SENSOR_PIN 54
  #define SPEED_SENSOR_UPDATE_FREQUENCY 5
  #define WHEEL_DIAM_CM 6.5f
  #define NR_MAGNETS 4
  #define WHEEL_CONSTANT PI*WHEEL_DIAM_CM/100.0 // 2*PI*r = 2*PI*diam/2 = PI*diam
  #define METERS_PER_PULSE WHEEL_CONSTANT/(float)NR_MAGNETS

  // Motor settings
  #define MOTOR_PIN 9

  // Steering settings
  #define STEERING_PIN 10
  
  // PID settings
  #define PID_SAMPLING_FREQUENCY 500
  #define PID_WINDOW_SIZE_SEC 5
  #define PID_WINDOW_SIZE_SAMP PID_WINDOW_SIZE_SEC*PID_SAMPLING_FREQUENCY
  #define ACCUMULATOR_RESET false

#endif
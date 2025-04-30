#ifndef CONSTANTS_H
#define CONSTANTS_H
  // Global settings
  #define DEBUG (1)
  #if DEBUG
    #define DEBUG_PRINT(x) Serial.println(x)
    #define DEBUG_PRINTLN(x) Serial.println(x)
    #define DEBUG_PRINTF(fmt, ...) { \
      char buffer[64]; /* Adjust buffer size as needed */ \
      snprintf(buffer, sizeof(buffer), fmt, __VA_ARGS__); \
      Serial.println(buffer); \
  }
  #else // No operation macros
    #define DEBUG_PRINT(x) 
    #define DEBUG_PRINTLN(x) 
    #define DEBUG_PRINTF(fmt, ...)
  #endif

  // Serial settings
  #define SERIAL_INPUT_BUFFER_SIZE 32
  #define SERIAL_FEEDBACK_FREQUENCY 10
  #define BAUD_RATE 115200

  // Speed sensor settings
  #define SPEED_SENSOR_PIN 54
  #define SPEED_SENSOR_UPDATE_FREQUENCY 10
  #define WHEEL_DIAM_CM 6.5f
  #define NR_MAGNETS 4
  #define WHEEL_CONSTANT PI*WHEEL_DIAM_CM/100.0 // 2*PI*r = 2*PI*diam/2 = PI*diam
  #define METERS_PER_PULSE WHEEL_CONSTANT/(float)NR_MAGNETS

  // Motor settings
  #define MOTOR_PIN 9

  // Steering settings
  #define STEERING_PIN 10
  
  // PID settings
  #define START_KP 10.0
  #define START_KI 20.0
  #define START_KD 3.0
  #define MAX_ACCUM_ERROR 100.0
  #define PID_SAMPLING_FREQUENCY 500
  #define PID_WINDOW_SIZE_SEC 10
  #define PID_WINDOW_SIZE_SAMP PID_WINDOW_SIZE_SEC*PID_SAMPLING_FREQUENCY
  #define ACCUMULATOR_RESET false

  // Proximity sensor settings
  #define PROXIMITY_PULSE_INTERVAL_US 1000000*2 // 2s
  #define PROXIMITY_PULSE_DURATION_US 10
  #define PROXIMITY_ECHO_PIN 39
  #define PROXIMITY_TRIGGER_PIN 50
  #define SPEED_OF_SOUND_CM_US 0.0343
#endif
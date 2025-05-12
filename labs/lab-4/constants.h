#ifndef CONSTANTS_H
#define CONSTANTS_H
  // Global settings
  #define DEBUG (1)
  #if DEBUG
    #define DEBUG_PRINT(x) Serial.print(x)
    #define DEBUG_PRINTLN(x) Serial.println(x)
    #define DEBUG_PRINTF(fmt, ...) { \
      char buffer[64]; \
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

  // Car and korridor geometry
  #define CAR_HJULBAS 0.265 // hjulbas i meter
  #define CAR_SENSORBREDD 0.165 // avstånd mellan ultraljus sensorer i meter
  #define KORRIDOR_BREDD 2.08 // bredden på korridoren i meter

  // Speed sensor settings
  #define SPEED_SENSOR_PIN 54
  #define SPEED_SENSOR_UPDATE_FREQUENCY SERIAL_FEEDBACK_FREQUENCY // Use serial timer
  #define WHEEL_DIAM_CM 6.5f
  #define NR_MAGNETS 4
  #define WHEEL_CONSTANT PI*WHEEL_DIAM_CM/100.0 // 2*PI*r = 2*PI*diam/2 = PI*diam
  #define METERS_PER_PULSE WHEEL_CONSTANT/(float)NR_MAGNETS

  // Motor settings
  #define MOTOR_PIN 9
  #define MIN_DISTANCE_TO_FRONT_WALL_CM 40

  // Steering settings
  #define STEERING_PIN 10
  #define STEERING_MAX_ANGLE_DEG 90.0
  #define STEERING_DISTANCE_TO_RIGHT_WALL_CM 50.0
  #define STEERING_MAP_MIN_TO_SERVO 0.0
  #define STEERING_MAP_MAX_TO_SERVO 50.0
  #define STEERING_MAP_MIN_FROM_SERVO -45.0
  #define STEERING_MAP_MAX_FROM_SERVO 45.0
  #define STEERING_MAP_CENTER_ANGLE 0
  
  // PID settings
  #define KP_SPEED 10.0
  #define KI_SPEED 20.0
  #define KD_SPEED 3.0
  #define KP_DIST 10.0
  #define KI_DIST 0.0
  #define KD_DIST 2.0
  #define MAX_ACCUM_ERROR 100.0
  #define PID_SAMPLING_FREQUENCY 500
  #define PID_WINDOW_SIZE_SEC 10
  #define PID_WINDOW_SIZE_SAMP PID_WINDOW_SIZE_SEC*PID_SAMPLING_FREQUENCY
  #define ACCUMULATOR_RESET false 

  // Proximity sensor settings
  #define PROXIMITY_SENSOR_UPDATE_FREQUENCY 50
  #define PROXIMITY_PULSE_DURATION_US 10
  #define RIGHT_PROXIMITY_ECHO_PIN 50
  #define RIGHT_PROXIMITY_TRIGGER_PIN 51  
  #define LEFT_PROXIMITY_ECHO_PIN 40
  #define LEFT_PROXIMITY_TRIGGER_PIN 41
  #define FORWARD_PROXIMITY_ECHO_PIN 30
  #define FORWARD_PROXIMITY_TRIGGER_PIN 31
  #define SPEED_OF_SOUND_CM_US 0.0343
#endif
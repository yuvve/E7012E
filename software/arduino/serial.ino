void setupSerialUSB(int baud) {
  Serial.begin(baud);
  DEBUG_PRINTLN("USB Serial initialized");
}

void setupSerialRock(int baud) {
  Serial1.begin(baud);
  DEBUG_PRINTLN("Rock Serial initialized");
}

Command parseSerialInput(const char* input) {
  char cmd = input[0];
  float value = atof(input + 1); // Convert the rest of the string to a floating-point number
  return {cmd, value};
}

void processSerialInput(const char* input) {
  Command cmd = parseSerialInput(input);
  switch (toUpperCase(cmd.cmd)) {
    case 'M':
      targetSpeed = cmd.value;
      if (targetSpeed <= 0.001){
        motorStarted = false;
        setTargetMotorRPMPercent(0.0);
        resetPID(&motorPID);
      } else {
        motorStarted = true;
      }
      break;
    case 'S':
      //targetCenterOffset = cmd.value;
      freeSteering(cmd.value);
      break;
    case 'P':
      adjustP(&distancePID, cmd.value);
      break;
    case 'I':
      adjustI(&distancePID, cmd.value);
      break;
    case 'D':
      adjustD(&distancePID, cmd.value);
      break;
    default:
      break;
  }
}

void readSerial(Stream& serialPort) {
  static char input[SERIAL_INPUT_BUFFER_SIZE];
  static size_t index = 0;

  while (serialPort.available()) {
    char c = serialPort.read();
    DEBUG_PRINT(c);

    if (c == '\n') {
      input[index] = '\0'; // Null-terminate the string
      processSerialInput(input);
      index = 0;
    } else if (index < sizeof(input) - 1) { // Check for buffer overflow
      input[index++] = c;
    } else { // Buffer overflow 
      index = 0;
      DEBUG_PRINTLN("Buffer overflow, resetting index");
    }
  }
}

void sendFeedback() {
  float currentSpeed = getSpeed();
  float leftRange = getProximityRange(leftProximitySensor);
  float rightRange = getProximityRange(rightProximitySensor);
  float forwardRange = getProximityRange(forwardProximitySensor);
  char message[SERIAL_INPUT_BUFFER_SIZE];
  snprintf(message, sizeof(message), "%.2f %.2f %.2f %.2f", 
    currentSpeed,forwardRange, leftRange, rightRange);

  Serial1.println(message);
  DEBUG_PRINTLN(message);
}

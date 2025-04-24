#define BUFFER_SIZE 32

void setupSerialUSB(int baud) {
  Serial.begin(baud);
  if DEBUG {
      Serial.println("USB Serial initialized");
  }
}

void setupSerialRock(int baud) {
  Serial1.begin(baud);
  if DEBUG {
    Serial.println("Serial to Rock initialized");
  }
}

Command parseSerialInput(const char* input) {
  char cmd = input[0];
  int value = atoi(input + 1); // Convert the rest of the string to an integer
  return Command {cmd, value};
}

void readSerial(Stream& serialPort) {
  static char input[BUFFER_SIZE];
  static size_t index = 0;

  while (serialPort.available()) {
    char c = serialPort.read();
    
    if (DEBUG) {
      Serial.print(c);
    }

    if (c == '\n') {
      input[index] = '\0'; // Null-terminate the string
      processSerialInput(input);
      index = 0;
    } else if (index < sizeof(input) - 1) { // Check for buffer overflow
      input[index++] = c;
    } else { // Buffer overflow 
      index = 0;
      if DEBUG {
        Serial.println("Buffer overflow, resetting index");
      }
    }
  }
}

void processSerialInput(const char* input) {
  Command cmd = parseSerialInput(input);
  switch (cmd.cmd) {
    case 'M':
      targetSpeed = cmd.value;
      break;
    case 'S':
      targetAngle = cmd.value;
      changeSteeringAngle(cmd.value);
      break;
    default:
      break;
  }
}

void writeSerial(Stream& serialPort, const char* message) {
  serialPort.println(message);
  serialPort.flush();
}

void sendFeedback() {
  float currentSpeed = getSpeed();
  char message[BUFFER_SIZE];
  snprintf(message, sizeof(message), "FB %.2f %.2f %.2f", currentSpeed, targetSpeed, targetAngle);

  writeSerial(Serial1, message);
  if (DEBUG) {
    writeSerial(Serial, message);
  }
}

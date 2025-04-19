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
  static char input[10];
  static int index = 0;

  while (serialPort.available()) {
    char c = serialPort.read();
    if (c == '\n') {
      input[index] = '\0'; // Null-terminate the string
      Command cmd = parseSerialInput(input);
      
      if (DEBUG) {
        Serial.print("Received serial: ");
        Serial.println(input);
      }

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

      memset(input, 0, sizeof(input)); // Clear the input buffer
      index = 0;
    } else {
      if (index < sizeof(input) - 1) { // Prevent buffer overflow
        input[index++] = c;
      } else { // Buffer overflow, reset buffer
        memset(input, 0, sizeof(input));
        index = 0;

        if DEBUG {
          Serial.println("Buffer overflow, resetting input buffer.");
        }
      }
    }
  }
}

void writeSerial(Stream& stream, const char* message) {
  stream.println(message);
  stream.flush();
}

void sendFeedback() {
  float current_speed = getSpeed();
  char message[50];
  snprintf(message, sizeof(message), "FB %.2f %.2f %.2f", current_speed, targetSpeed, targetAngle);

  writeSerial(Serial1, message);
  if (DEBUG) {
    writeSerial(Serial, message);
  }
}

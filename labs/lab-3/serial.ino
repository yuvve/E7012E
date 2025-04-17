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

Command parseSerialInput(String input) {
  char cmd = input.charAt(0);
  int value = input.substring(1).toInt();
  return Command {cmd, value};
}

void sendFeedback() {
  float current_speed = getSpeed();
  Serial1.print("FB ");
  Serial1.print(current_speed);
  Serial1.print(" ");
  Serial1.print(targetSpeed);
  Serial1.print(" ");
  Serial1.println(targetAngle);
  Serial1.flush();

  if DEBUG {
    Serial.print("FB ");
    Serial.print(current_speed);
    Serial.print(" ");
    Serial.print(targetSpeed);
    Serial.print(" ");
    Serial.println(targetAngle);
    Serial.flush();
  }
}

void processSerial(Stream& serialPort) {
  static String input;

  while (serialPort.available()) {
    char c = serialPort.read();
    if (c == '\n') {
      input.trim();
      Command cmd = parseSerialInput(input);

      switch (cmd.cmd) {
        case 'M':
          targetSpeed = cmd.value;
          sendFeedback();

          if (DEBUG) {
            Serial.print(cmd.cmd);
            Serial.println(cmd.value);
          }
          break;

        case 'S':
          targetAngle = cmd.value;
          changeSteeringAngle(cmd.value);
          sendFeedback();

          if (DEBUG) {
            Serial.print(cmd.cmd);
            Serial.println(cmd.value);
          }
          break;
        default:
          break;
      }

      input = "";
    } else {
      input += c;
    }
  }
}

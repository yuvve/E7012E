void setupSerial() {
    Serial.begin(9600);
  if DEBUG {
      Serial.println("Serial initialized");
  }
}

Command parseSerialInput(String input) {
  char cmd = input.charAt(0);
  int value = input.substring(1).toInt();
  return Command {cmd, value};
}
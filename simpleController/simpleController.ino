/*
Command list:
s(115): single click

d(100): double click

m(109): move mouse position (with two following integer separated by ',' end with '\n')
  - x: relative x moving
  - y: relative y moving
  - Example: m240,31\n

e(101): keyboard write 'Enter'

k(107): key in a certain character (with one following ASCII code)
*/
long CLICK_FREQUENCY = 100;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 0) {
    // Start with a command character
    char c = (char)Serial.read();

    if (c == 'm') {
      // Serial.println("[Command] move");
      long x = Serial.parseInt();
      Serial.read();  // consume ',' in the buffer
      long y = Serial.parseInt();
      Move(x, y);

    } else if (c == 's') {
      // Serial.println("[Command] single click");
      Click();

    } else if (c == 'd') {
      // Serial.println("[Command] double click");
      MultiClick(2, CLICK_FREQUENCY);

    } else {
      Serial.print("[Error] Invalid command: ");
      Serial.println(c);
    }
    CleanBuffer();
  }
}

void Move(long x, long y) {
  Serial.print("[Mouse] moving (");
  Serial.print(x);
  Serial.print(",");
  Serial.print(y);
  Serial.println(")");
}

void Click() {
  Serial.println("[Mouse] click");
}

void MultiClick(int times, long duration) {
  for (int i = 0; i < times - 1; i++) {
    Click();
    delay(duration);
  }
  Click();
}

void CleanBuffer() {
  while (Serial.read() != -1) {
  }
}
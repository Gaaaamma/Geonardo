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
#include <Keyboard.h>
#include <Mouse.h>
long CLICK_FREQUENCY = 100;
long MOVE_STEP = 30;  // 0 < MOVE_STEP < 128
const char ENTER = 13;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Mouse.begin();
  Keyboard.begin();
}

void loop() {  
  if (Serial.available() > 0) {
    // Start with a command character
    char c = (char)Serial.read();

    if (c == 'm') {
      Serial.println("[Command] move");
      long x = Serial.parseInt();
      Serial.read();  // consume ',' in the buffer
      long y = Serial.parseInt();
      LongMove(x, y);

    } else if (c == 's') {
      Serial.println("[Command] single click");
      Click(MOUSE_LEFT);

    } else if (c == 'd') {
      Serial.println("[Command] double click");
      MultiClick(2, CLICK_FREQUENCY, MOUSE_LEFT);

    } else if (c == 'e') {
      Serial.println("[Command] key enter");
      KeyStroke(ENTER);

    } else if (c == 'k') {
      Serial.println("[Command] key something");
      char key = (char)Serial.read();
      if (key != 0) {
        KeyStroke(key);
      }

    } else {
      Serial.println("[Error] Invalid command");
    }
    CleanBuffer();
  }
}

void LongMove(long x, long y) {
  // Moving distance might over one step => divide it into multiple pieces
  long xTimes = x / MOVE_STEP;
  long yTimes = y / MOVE_STEP;

  long xRemain = x - (xTimes * MOVE_STEP);
  long yRemain = y - (yTimes * MOVE_STEP);

  // Move X
  long direction = (xTimes >= 0) ? 1 : -1;
  for (long i = 0; i < abs(xTimes); i++) {
    Move((signed char)(MOVE_STEP * direction), 0);
  }
  if (xRemain != 0) {
    Move((signed char)xRemain, 0);
  }

  // Move Y
  direction = (yTimes >= 0) ? 1 : -1;
  for (long i = 0; i < abs(yTimes); i++) {
    Move(0, (signed char)(MOVE_STEP * direction));
  }
  if (yRemain != 0) {
    Move(0, (signed char)yRemain);
  }
}

void MoveLikeHuman(long x, long y) {
  float ratio = (float)x / (float) y;
  Serial.println(ratio);
}

void Move(signed char x, signed char y) {
  // Must use a switch or some controller to keep it safe
  Mouse.move(x, y);
}

void Click(uint8_t b) {
  // Must use a switch or some controller to keep it safe
  Mouse.click(b);
}

void MultiClick(int times, long duration, uint8_t b) {
  for (int i = 0; i < times - 1; i++) {
    Click(b);
    delay(duration);
  }
  Click(b);
}

void KeyStroke(char c) {
  Keyboard.write(c);
}

void CleanBuffer() {
  while (Serial.read() != -1) {
  }
}
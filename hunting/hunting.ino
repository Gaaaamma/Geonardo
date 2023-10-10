/*
KEY_UP_ARROW
KEY_DOWN_ARROW
KEY_LEFT_ARROW
KEY_RIGHT_ARROW
*/
#include "HID-Project.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Keyboard.begin();
  randomSeed(analogRead(0));
}

void loop() {
  if (Serial.available() > 0) {
    // Start with a command character
    delay(3000);
    char c = (char)Serial.read();
    if (c == '1') {
      DoubleJumpAttackA(KEY_RIGHT_ARROW, 3, 100, 'f', 'd', KEY_RIGHT_ARROW);
    } else if (c == '2') {
      JumpAttackA(KEY_LEFT_ARROW, 5, 500, 'f', 'd');
    } else if (c == '3') {
      DoubleJump('f', 'a', 100);
    }
  }
}

void DoubleJumpAttackA(char direction, int times, long latency, char jump, char attack, char useless) {
  Keyboard.press(direction);
  for (int i = 0; i < times; i++) {
    DoubleJump(jump, useless, latency);
    delay(100);
    Keyboard.write(attack);
    delay(1000);
  }
  Keyboard.releaseAll();
}

void JumpAttackA(char direction, int times, long latency, char jump, char attack) {
  Keyboard.press(direction);
  for (int i = 0; i < times; i++) {
    Keyboard.write(jump);
    delay(100);
    Keyboard.write(attack);
    delay(latency);
  }
  Keyboard.releaseAll();
}

void DoubleJump(char jump, char useless, long latency) {
  Keyboard.write(jump);
  delay(latency);
  Keyboard.write(useless);
  delay(100);
  Keyboard.write(jump);
}

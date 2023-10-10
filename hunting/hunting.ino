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
      DoubleJumpAttackA(true, 'f', 'd');
    } else if (c == '2') {
      DoubleJumpAttackA(false, 'f', 'd');
    } else if (c == '3') {
      UpJump('f');
    }
  }
}


void DoubleJumpAttackA(bool direction, char jump, char attack) {
  if (direction == true) {
    Keyboard.press(KEY_RIGHT_ARROW);
  } else {
    Keyboard.press(KEY_LEFT_ARROW);
  }
  for (int i = 0; i < 3; i++) {
    DoubleJump(jump);
    delay(100);
    Keyboard.write(attack);
    delay(600); // 600 ~ 
  }
  Keyboard.releaseAll();
}

void DoubleJump(char jump) {
  Keyboard.write(jump);
  delay(100);
  Keyboard.write(jump);
  Keyboard.write(jump);
}
void UpJump(char jump) {
  //Keyboard.press(KEY_UP_ARROW);
  //Keyboard.press(jump);
  //delay(100);
  //Keyboard.release(jump);
  //Keyboard.write(jump);
  //Keyboard.releaseAll();
}

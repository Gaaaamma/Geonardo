/*
KEY_UP_ARROW
KEY_DOWN_ARROW
KEY_LEFT_ARROW
KEY_RIGHT_ARROW
*/
#include "HID-Project.h"

const char JUMP = 'f';
const char WIND_MOVE = 'w';
const char ELF_SHIELD = 'd';
const char SONG_SKY = 'a';
const char ROPE = 't';

const char FOUNTAIN = 'z';
const unsigned long FOUNTAIN_CD = 58; 
const char TORNADO = 'b';
const unsigned long TORNADO_CD = 20; 
const char SWIRL = 'e';
const unsigned long SWIRL_CD = 30;
const char FANTASY = ' ';
const unsigned long FANTASY_CD = 10;
const char BIRD = 'v';
const unsigned long BIRD_CD = 30;

const char BUFF_3 = '3';
const unsigned long BUFF_3_CD = 120; 
const char BUFF_4 = '4';
const unsigned long BUFF_4_CD = 120; 
const char BUFF_G = 'g';
const unsigned long BUFF_G_CD = 90;
const char BUFF_C = 'c';
const unsigned long BUFF_C_CD = 150;
const char BUFF_X = 'x';
const unsigned long BUFF_X_CD = 120; 

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
    } else if (c == '0') {
      bool direction = false;
      int times = 5;
      Turn(direction);
      UpJump(400, 450);
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

void Turn(bool direction) {
  if (direction == true) {
    Keyboard.press(KEY_RIGHT_ARROW);
  } else {
    Keyboard.press(KEY_LEFT_ARROW);
  }
  delay(30);
  Keyboard.releaseAll();
  delay(100);
}

// min max will affect the height of UpJump
void UpJump(unsigned long min, unsigned long max) {
  Keyboard.write(JUMP);  
  Keyboard.press(KEY_UP_ARROW);
  delay(random(min, max));

  Keyboard.write(JUMP);
  Keyboard.write(JUMP);
  delay(random(50, 100));
  Keyboard.releaseAll();
}

void DownJump() {
  Keyboard.press(KEY_DOWN_ARROW);
  Keyboard.write(JUMP);
  delay(random(50, 100));
  Keyboard.releaseAll();
}

// min: 90, max: 120 (Round_min: 700, Round_max: 900)
void DoubleJumpLatency(bool direction, unsigned long min, unsigned long max) {
  if (direction == true) {
    Keyboard.press(KEY_RIGHT_ARROW);
  } else {
    Keyboard.press(KEY_LEFT_ARROW);
  }
  Keyboard.write(JUMP);
  delay(random(min, max));
  Keyboard.write(JUMP);
  Keyboard.write(JUMP);
  Keyboard.releaseAll();
}

// min: 80, max: 120 (Round_min + Margin: 800, Margin: 0 ~ 300)
void DoubleJumpAttackLatency(bool direction, unsigned long min, unsigned long max, unsigned long margin) {
  if (direction == true) {
    Keyboard.press(KEY_RIGHT_ARROW);
  } else {
    Keyboard.press(KEY_LEFT_ARROW);
  }
  Keyboard.write(JUMP);
  delay(random(min, max));
  Keyboard.write(JUMP);
  Keyboard.write(JUMP);
  delay(margin);
  Keyboard.write(ELF_SHIELD);
  Keyboard.releaseAll();
}

// min: 80, max: 120 (Round_min: 550, Round_max: 800)
void WindMove(bool direction, unsigned long min, unsigned long max) {
  if (direction == true) {
    Keyboard.press(KEY_RIGHT_ARROW);
  } else {
    Keyboard.press(KEY_LEFT_ARROW);
  }
  delay(random(min, max));
  Keyboard.write(WIND_MOVE);
  Keyboard.releaseAll();
}

// min: 80, max: 120 (Round_min: 900, Margin: 100 ~ 500)
// margin: 100 moves the shortest
// margin: 500 moves the longest
void DoubleJumpWindMove(bool direction, unsigned long min, unsigned long max) {
  DoubleJumpLatency(direction, min, max);
  delay(random(100, 500)); 
  WindMove(direction, min, max);
}

// min: 80, max: 120 (Round_min: 50, Duration: 100 ~ 1000)
unsigned long SongOfTheSky(bool direction, unsigned long min, unsigned long max, unsigned long duration) {
  if (direction == true) {
    Keyboard.press(KEY_RIGHT_ARROW);
  } else {
    Keyboard.press(KEY_LEFT_ARROW);
  }
  unsigned long distance = random(min, max);
  delay(distance);
  Keyboard.press(SONG_SKY);
  delay(random(99, duration));
  Keyboard.releaseAll();
  return distance;
}

void Fountain(bool direction) {
  if (direction == true) {
    Keyboard.press(KEY_RIGHT_ARROW);
  } else {
    Keyboard.press(KEY_LEFT_ARROW);
  }
  delay(random(50, 100));
  Keyboard.releaseAll();

  Keyboard.press(KEY_DOWN_ARROW);
  Keyboard.write(FOUNTAIN);
  delay(random(50, 100));
  Keyboard.releaseAll();
}

void Tornado(bool direction) {
  if (direction == true) {
    Keyboard.press(KEY_RIGHT_ARROW);
  } else {
    Keyboard.press(KEY_LEFT_ARROW);
  }
  delay(random(50, 100));
  Keyboard.write(TORNADO);
  Keyboard.releaseAll();
}

void Swirl(bool direction) {
  if (direction == true) {
    Keyboard.press(KEY_RIGHT_ARROW);
  } else {
    Keyboard.press(KEY_LEFT_ARROW);
  }
  delay(random(50, 100));
  Keyboard.write(SWIRL);
  Keyboard.releaseAll();
}

void SimpleSkill(bool direction, char Command) {
  if (direction == true) {
    Keyboard.press(KEY_RIGHT_ARROW);
  } else {
    Keyboard.press(KEY_LEFT_ARROW);
  }
  delay(random(50, 70));
  Keyboard.write(Command);
  Keyboard.releaseAll();
}

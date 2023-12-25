/*
KEY_UP_ARROW
KEY_DOWN_ARROW
KEY_LEFT_ARROW
KEY_RIGHT_ARROW
*/
/*               HID AbsoluteMouse coordination 
(-32767, -32767) ------------- # ------------- (32767, -32767)
                 |             |             |
                 |             |             |
                 |------------ 0 ------------| (32767, 0)
                 |             |             |
                 |             |             |
(-32767, 32767)  ------------- # ------------- (32767, 32767)
*/

#include "HID-Project.h"

long CLICK_FREQUENCY = 100;
const char ENTER = 13;

int currentX = 0;
int currentY = 0;

const long MOVE_BOUND_MAX = 32767;
const long MOVE_BOUND_MIN = -32767;

const char BOSS_UI = '.';
const int UI_ROW_STARTX = 0;
const int UI_ROW_STARTY = 0;
const long UI_ROW_DISTANCE = 2000;
const int BOSS_CONFIRM_X = 0;
const int BOSS_CONFIRM_Y = 0;

const unsigned long WHEEL_CD = 900;
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

const unsigned long MONEY_CD = 90; 

const int BUFF_COUNTS = 5;
const char BUFF[5] = {'3', '4', 'g', 'c', 'x'};
const String BUFF_NAME[5] = {"Storm", "Glory", "Shilff", "Grandpa", "Critical"};
unsigned long BUFF_CD[5] = {120, 120, 90, 150, 120};

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
      UIChoose(0, false);
    } else if (c == '2') {
      UIChoose(1, true);
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

// minUp maxUp will affect the height of UpJump
void UpJump(unsigned long minUp, unsigned long maxUp) {
  Keyboard.write(JUMP);  
  Keyboard.press(KEY_UP_ARROW);
  delay(random(minUp, maxUp));

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

// minDJ: 90, maxDJ: 120 (Round_min: 700, Round_max: 900)
void DoubleJumpLatency(bool direction, unsigned long minDJ, unsigned long maxDJ) {
  if (direction == true) {
    Keyboard.press(KEY_RIGHT_ARROW);
  } else {
    Keyboard.press(KEY_LEFT_ARROW);
  }
  Keyboard.write(JUMP);
  delay(random(minDJ, maxDJ));
  Keyboard.write(JUMP);
  Keyboard.write(JUMP);
  Keyboard.releaseAll();
}

// minDJ: 80, maxDJ: 120 (Round_min + minAttack: >800, Attack: 0 ~ 300)
void DoubleJumpAttackLatency(bool direction, unsigned long minDJ, unsigned long maxDJ, unsigned long minAttack, unsigned long maxAttack) {
  if (direction == true) {
    Keyboard.press(KEY_RIGHT_ARROW);
  } else {
    Keyboard.press(KEY_LEFT_ARROW);
  }
  Keyboard.write(JUMP);
  delay(random(minDJ, maxDJ));
  Keyboard.write(JUMP);
  Keyboard.write(JUMP);
  delay(random(minAttack, maxAttack));
  Keyboard.write(ELF_SHIELD);
  Keyboard.releaseAll();
}

// minWalk: 80, maxWalk: 120 (Round_min: 550, Round_max: 800)
void WindMove(bool direction, unsigned long minWalk, unsigned long maxWalk) {
  if (direction == true) {
    Keyboard.press(KEY_RIGHT_ARROW);
  } else {
    Keyboard.press(KEY_LEFT_ARROW);
  }
  delay(random(minWalk, maxWalk));
  Keyboard.write(WIND_MOVE);
  Keyboard.releaseAll();
}

// minDJ: 80, maxDJ: 120 (Round_min: 900, Margin: 100 ~ 500)
// margin: 100 moves the shortest
// margin: 500 moves the longest
void DoubleJumpWindMove(bool direction, unsigned long minDJ, unsigned long maxDJ, unsigned long minMargin, unsigned long maxMargin, unsigned long minWalk, unsigned long maxWalk) {
  DoubleJumpLatency(direction, minDJ, maxDJ);
  delay(random(minMargin, maxMargin)); 
  WindMove(direction, minWalk, maxWalk);
}

// minWalk: 80, maxWalk: 120 (Round_min: 50, Duration: >100)
unsigned long SongOfTheSky(bool direction, unsigned long minWalk, unsigned long maxWalk, unsigned long minDuration, unsigned long maxDuration) {
  if (direction == true) {
    Keyboard.press(KEY_RIGHT_ARROW);
  } else {
    Keyboard.press(KEY_LEFT_ARROW);
  }
  unsigned long distance = random(minWalk, maxWalk);
  delay(distance);
  Keyboard.press(SONG_SKY);
  delay(random(minDuration, maxDuration));
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
  delay(random(70, 100));
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
  //if (direction == true) {
  //  Keyboard.press(KEY_RIGHT_ARROW);
  //} else {
  //  Keyboard.press(KEY_LEFT_ARROW);
  //}
  //delay(random(50, 70));
  Keyboard.write(Command);
  //Keyboard.releaseAll();
}

void Move(char direction[], int counts, unsigned long minDelay[], unsigned long maxDelay[]){
  for (int i = 0; i < counts; i++) {
    if (direction[i] == 'w') {
      UpJump(300, 350);
    } else if (direction[i] == 'a') {
      WindMove(false, 80, 100);
    } else if (direction[i] == 's') {
      DownJump();
    } else if (direction[i] == 'd') {
      WindMove(true, 80, 100);
    } else if (direction[i] == 'q') {
      DoubleJumpLatency(false, 90, 120);
    } else if (direction[i] == 'e') {
      DoubleJumpLatency(true, 90, 120);
    } else if (direction[i] == 'l') {
      Turn(false);
    } else if (direction[i] == 'r') {
      Turn(true);
    } else if (direction[i] == 'j') {
      DoubleJumpAttackLatency(false, 80, 100, 100, 200);
    } else if (direction[i] == 'k') {
      DoubleJumpAttackLatency(true, 80, 100, 100, 200);
    }
    delay(random(minDelay[i], maxDelay[i]));
  }
}

/************** 2-6 ***************/
void MoveToFountain_2_6() {
  unsigned long rand = random(4);
  if (rand == 0) {
    MoveToFountainA_2_6();
  } else if (rand == 1) {
    MoveToFountainB_2_6();
  } else if (rand == 2) {
    MoveToFountainC_2_6();
  } else if (rand == 3) {
    MoveToFountainD_2_6();
  }
}

void BackFromFountain_2_6() {
  unsigned long rand = random(2);
  if (rand == 0) {
    BackFromFountainA_2_6();
  } else if (rand == 1) {
    BackFromFountainB_2_6();
  } 
}

void MoveToFountainA_2_6() {
  char commands[] = {'a', 'a', 'w', 'r'};
  unsigned long minDelay[] = {550, 650, 900, 50};
  unsigned long maxDelay[] = {650, 750, 1000, 80};
  Move(commands, 4, minDelay, maxDelay);
}

void MoveToFountainB_2_6() {
  char commands[] = {'w', 'a', 'a', 'r'};
  unsigned long minDelay[] = {700, 550, 550, 250};
  unsigned long maxDelay[] = {750, 600, 600, 280};
  Move(commands, 4, minDelay, maxDelay);
}

void MoveToFountainC_2_6() {
  char commands[] = {'a', 'w', 'a', 'r'};
  unsigned long minDelay[] = {550, 700, 550, 250};
  unsigned long maxDelay[] = {600, 750, 600, 280};
  Move(commands, 4, minDelay, maxDelay);
}

void MoveToFountainD_2_6() {
  char commands[] = {'q', 'w', 'r'};
  unsigned long minDelay[] = {700, 400, 1000};
  unsigned long maxDelay[] = {750, 500, 1200};
  Move(commands, 3, minDelay, maxDelay);
}

void BackFromFountainA_2_6() {
  char commands[] = {'e', 'd'};
  unsigned long minDelay[] = {350, 1000};
  unsigned long maxDelay[] = {450, 1200};
  Move(commands, 2, minDelay, maxDelay);
}

void BackFromFountainB_2_6() {
  char commands[] = {'s', 'd', 'd'};
  unsigned long minDelay[] = {1100, 550, 700};
  unsigned long maxDelay[] = {1200, 600, 800};
  Move(commands, 3, minDelay, maxDelay);
}

void CollectMoney_2_6() {
  unsigned long rand = random(1);
  if (rand == 0) {
    CollectMoneyA_2_6();
  } else if (rand == 1) {

  }
}

void CollectMoneyA_2_6() {
  char commands[] = {'d', 'd', 'w', 'l', 'q', 'a'};
  unsigned long minDelay[] = {550, 550, 900, 200, 350, 1000};
  unsigned long maxDelay[] = {650, 650, 1000, 250, 450, 1200};
  Move(commands, 6, minDelay, maxDelay);
}

/************** Basic command ***************/
void Click(uint8_t b) {
  AbsoluteMouse.click(b);
}

void KeyStroke(char c) {
  Keyboard.write(c);
}

/************** BOSS list ***************/
void UIChoose(int index, bool nextPage) {
  // Open BOSS UI and choose the boss to move
  KeyStroke(BOSS_UI);
  delay(1000);

  int targetY = UI_ROW_STARTY + index * (int)UI_ROW_DISTANCE;
  if (nextPage) {
    AbsoluteMouse.scroll(UI_ROW_STARTX, targetY, 1);
  } else {
    AbsoluteMouse.moveTo(UI_ROW_STARTX, targetY);
  }  
  delay(1000);

  Click(MOUSE_LEFT);
  delay(1000);

  AbsoluteMouse.moveto(BOSS_CONFIRM_X, BOSS_CONFIRM_Y)
  Click(MOUSE_LEFT);
  delay(1000);
}

void Zakum() {

}

void Magnus() {

}

void Hilla() {

}

void Big4() {

}

void BloodyQueen() {

}

void Pierre() {

} 

void Bon() {

}

void Vellum() {

}

void VonLeon() {

}

void Horntail() {

}

void Arkarium() {

}

void PinkBean() {

}
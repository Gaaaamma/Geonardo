/*
KEY_UP_ARROW
KEY_DOWN_ARROW
KEY_LEFT_ARROW
KEY_RIGHT_ARROW
*/
#include "HID-Project.h"

// 輪迴
const char FRENZY = 'q';
const long FRENZY_CD = 360;

// 燃燒 
const char BURN = 'e'; 
const long BURN_CD = 1830;

// Basic setting
const int START_WAIT = 10;
const char JUMP = 'f';
const char ROPE = 't';
const char CONFIRM = 'y';
const char ENTER = 10;
const char GUIDE = 'u';
const long GUIDE_FIRST_X = -30000;
const long GUIDE_FIRST_Y = -2000;
const long GUIDE_DISTANCE_X = 1650;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Keyboard.begin();
  randomSeed(analogRead(0));
}

void loop() {
  if (Serial.available() > 0) {
    // Start with a command character
    delay(500);
    char c = (char)Serial.read();
    bool frenzy_on = false;
    bool burn_on = false;
    int minutes = 0;
    char input = ' ';

    if (c == 's') { // start
      // Frenzy
      ConsumeInput();
      while (true) {      
        Serial.print("啟用輪迴: (y/n) ");
        WaitInput();
        input = (char)Serial.read();
        ConsumeInput();
        if (input == 'y') {
          Serial.println("YES");
          frenzy_on = true;
          break;
        } else if (input == 'n') {
          Serial.println("NO");
          frenzy_on = false;
          break;
        } else {
          Serial.println("?");
        }
      }

      // Burn
      while (true) {      
        Serial.print("啟用燃燒: (y/n) ");
        WaitInput();
        input = (char)Serial.read();
        ConsumeInput();
        if (input == 'y') {
          Serial.println("YES");
          burn_on = true;
          break;
        } else if (input == 'n') {
          Serial.println("NO");
          burn_on = false;
          break;
        } else {
          Serial.println("?");
        }
      }
      
      // Time setting
      while (true) {     
        Serial.print("持續時間: (1 ~ 360min) ");
        WaitInput();
        minutes = Serial.parseInt();
        ConsumeInput();
        if (minutes > 0 && minutes <= 360) {
          Serial.print(minutes);
          Serial.println(" min");
          break;
        }
        Serial.println("?");
      }
      ShowInfo(frenzy_on, burn_on, minutes);

      // Confirm
      bool confirm = false;
      while (true) {      
        Serial.print("設定正確: (y/n) ");
        WaitInput();
        input = (char)Serial.read();
        ConsumeInput();
        if (input == 'y') {
          Serial.println("YES");
          confirm = true;
          break;
        } else if (input == 'n') {
          Serial.println("NO");
          Serial.println("請重新設定");
          break;
        } else {
          Serial.println("?");
        }
      }

      // Timer start
      if (confirm == true) {
        Serial.print(START_WAIT);
        Serial.println("秒後開始執行 請返回遊戲內等待");
        for (int i = START_WAIT; i > 0; i--) {
          Serial.print(i);
          Serial.print(" ");
          delay(1000);
        }
        Serial.println();

        unsigned long start = millis();
        unsigned long FrenzyStart = start;
        unsigned long BurnStart = start;
        unsigned long time = millis();
        int second = (time-start)/1000;
        bool startUp = true;
        char notRobot[] = {'z', 'x'};
        unsigned long minDelay[] = {300, 300};
        unsigned long maxDelay[] = {350, 350};

        while (second < minutes * 60) {
          // Frenzys
          if (frenzy_on && (startUp || (time - FrenzyStart)/1000 > FRENZY_CD)) {
            FrenzyStart = millis();
            Move(notRobot, 2, minDelay, maxDelay);
            SkillUse('f', (FrenzyStart - start) / 1000 / 60);
            
            // frenzy delay
            delay(3000);
          }

          // Burn
          if (burn_on && (startUp || (time - BurnStart)/1000 > BURN_CD)) {
            BurnStart = millis();
            Move(notRobot, 2, minDelay, maxDelay);
            SkillUse('b', (BurnStart - start) / 1000 / 60);
            
            
            // burn delay
            delay(3000);
          }

          // time set
          time = millis();
          second = (time-start)/1000;
          startUp = false;
        }
      }
    } else if (c == 't') {
      // Stand for testing
      Serial.println("Nothing to test");
      ConsumeInput();

    } else {
      Serial.println("Help:\n  key 's' for start\n  key 't' for testing");
      ConsumeInput();
    }
    } 
}

void ShowInfo(bool frenzy_on, bool burn_on, int minutes) {
  Serial.println("\n|====================|");
  Serial.print("|  啟用輪迴: ");
  if (frenzy_on == true) {
    Serial.println("true");
  } else {
    Serial.println("false");
  }
  Serial.print("|  啟用燃燒: ");
  if (burn_on == true) {
    Serial.println("true");
  } else {
    Serial.println("false");
  }
  Serial.print("|  持續時間: ");
  Serial.print(minutes);
  Serial.println(" min");
  Serial.println("|====================|\n");
}

void SkillUse(char type, int minute) {
  if (type == 'f') {
    Serial.print("|  施放輪迴: ");
    Serial.print(minute);
    Serial.println(" min");
    SimpleSkill(FRENZY, 500);

  } else if (type == 'b') {
    Serial.print("|  施放燃燒: ");
    Serial.print(minute);
    Serial.println(" min");
    SimpleSkill(BURN, 500);

  }
}

void DoubleJump(char jump) {
  Keyboard.write(jump);
  delay(100);
  Keyboard.write(jump);
  Keyboard.write(jump);
}

void WalkLatency(bool direction, unsigned long minLatency, unsigned long maxLatency) {
  if (direction == true) {
    Keyboard.press(KEY_RIGHT_ARROW);
  } else {
    Keyboard.press(KEY_LEFT_ARROW);
  }
  delay(random(minLatency, maxLatency));
  Keyboard.releaseAll();
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
  delay(random(80, 100));
  Keyboard.releaseAll();
}

void DownJump() {
  Keyboard.press(KEY_DOWN_ARROW);
  Keyboard.write(JUMP);
  delay(random(80, 100));
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

void SimpleSkill(char Command, long time) {
  Keyboard.write(Command);
  delay(time);
}

void Move(char direction[], int counts, unsigned long minDelay[], unsigned long maxDelay[]){
  for (int i = 0; i < counts; i++) {
    if (direction[i] == 'w') {
      UpJump(300, 350);
    } else if (direction[i] == 'a') {

    } else if (direction[i] == 's') {
      DownJump();
    } else if (direction[i] == 'd') {

    } else if (direction[i] == 'q') {
      DoubleJumpLatency(false, 90, 120);
    } else if (direction[i] == 'e') {
      DoubleJumpLatency(true, 90, 120);
    } else if (direction[i] == 'l') {
      Turn(false);
    } else if (direction[i] == 'r') {
      Turn(true);
    } else if (direction[i] == 'z') {
      WalkLatency(false, 200, 250);
    } else if (direction[i] == 'x') {
      WalkLatency(true, 200, 250);
    }
    delay(random(minDelay[i], maxDelay[i]));
  }
}

void ArrowMove(char direction[], int counts, unsigned long wait[]) {
  for (int i = 0; i < counts; i++) {
    switch (direction[i]) {
    case 'w':
      Keyboard.write(KEY_UP_ARROW);
      delay(wait[i]);
      break;
    case 's':
      Keyboard.write(KEY_DOWN_ARROW);
      delay(wait[i]);
      break;
    case 'a':
      Keyboard.write(KEY_LEFT_ARROW);
      delay(wait[i]);
      break;
    case 'd':
      Keyboard.write(KEY_RIGHT_ARROW);
      delay(wait[i]);
      break;
    case 'y':
      Keyboard.write(CONFIRM);
      delay(wait[i]);
      break;
    case 'e':
      Keyboard.write(ENTER);
      delay(wait[i]);
      break;
    
    default:
      break;
    }
  }
}

void GuideMoving(int index) {
  // Open UI
  Keyboard.write(GUIDE);
  delay(1000);
  // Move mouse to the first location
  AbsoluteMouse.moveTo(GUIDE_FIRST_X, GUIDE_FIRST_Y);
  delay(1000);
  
  // Move right according to the index value
  for (int i = 0; i < index; i++) {
    AbsoluteMouse.move(GUIDE_DISTANCE_X, 0);
    delay(200);
  }

  // Move and wait
  AbsoluteMouse.click(MOUSE_LEFT);
  delay(2000);
}

void WaitInput() {
  for (;Serial.available() <= 0;) {
    // Waiting input
  }
}

void ConsumeInput() {
  while (Serial.available()  > 0) {
    char c = Serial.read();
  }
}
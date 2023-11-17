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
    } else if (c == '2') {
      DoubleJumpAttackA(false, 'f', 'd');

    } else if (c == '3') {
      MoveFountainA();
      BackFountainA();
      MoveFountainA();
      BackFountainA();

    } else if (c == '0') {
      bool direction = false;
      unsigned long start = millis();
      unsigned long TornadoStart = start;
      unsigned long SwirlStart = start;
      unsigned long FantasyStart = start;
      unsigned long FountainStart = start;
      unsigned long BirdStart = start;
      unsigned long buffStart[5] = {start, start, start, start, start};

      for (int i = 0; i < 500; i++) {
        SongOfTheSky(direction, 80, 120, 100, 1000);
        direction = !direction;
        
        unsigned long time = millis();
        int second = (time-start)/1000;
        // Fantasy
        if ((time-FantasyStart)/1000 > FANTASY_CD) {
          SimpleSkill(direction, FANTASY);
          FantasyStart = millis();
          delay(600);
          Serial.print(second);
          Serial.println("Fantasy");
        }
        // Tornado
        if ((time-TornadoStart)/1000 > TORNADO_CD) {
          Tornado(direction);
          TornadoStart = millis();
          delay(random(700, 1000));

          Serial.print(second);
          Serial.println("Tornado");
        }

        time = millis();
        second = (time-start)/1000;
        // Swirl
        if ((time-SwirlStart)/1000 > SWIRL_CD) {
          Swirl(direction);
          SwirlStart = millis();
          delay(random(800, 1000));

          Serial.print(second);
          Serial.println("Swirl");
        }
        // Bird
        if ((time-BirdStart)/1000 > BIRD_CD) {
          SimpleSkill(direction, BIRD);
          BirdStart = millis();
          delay(random(800, 1000));

          Serial.print(second);
          Serial.println("Bird");
        }

        // Buffs
        for (int i = 0; i < BUFF_COUNTS; i++) {
          second = (millis()-start)/1000;
          if ((time-buffStart[i])/1000 > BUFF_CD[i]) {
            SimpleSkill(direction, BUFF[i]);
            buffStart[i] = millis();
            delay(random(800, 1000));

            Serial.print(second);
            Serial.println(BUFF_NAME[i]);
          }
        }

        time = millis();
        second = (time-start)/1000;
        // Fountain
        if ((time-FountainStart)/1000 > FOUNTAIN_CD) {
          // Move to the specific position

          // Fountain
          Fountain((bool)random(2));
          FountainStart = millis();
          delay(random(800, 1000));

          Serial.print(second);
          Serial.println("Fountain");
          
          // Move back to origin position
        }
        delay(random(50, 100));
      }
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
unsigned long SongOfTheSky(bool direction, unsigned long min, unsigned long max, unsigned long minDuration, unsigned long maxDuration) {
  if (direction == true) {
    Keyboard.press(KEY_RIGHT_ARROW);
  } else {
    Keyboard.press(KEY_LEFT_ARROW);
  }
  unsigned long distance = random(min, max);
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
    }
    delay(random(minDelay[i], maxDelay[i]));
  }
}

void MoveFountainA() {
  char commands[] = {'a', 'a', 'w', 'r'};
  unsigned long minDelay[] = {550, 550, 900, 50};
  unsigned long maxDelay[] = {650, 650, 1000, 80};
  Move(commands, 4, minDelay, maxDelay);
}
void BackFountainA() {
  char commands[] = {'e'};
  unsigned long minDelay[] = {1500};
  unsigned long maxDelay[] = {2000};
  Move(commands, 1, minDelay, maxDelay);
}
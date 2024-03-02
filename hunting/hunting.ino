/*
KEY_UP_ARROWc
KEY_DOWN_ARROW
KEY_LEFT_ARROW
KEY_RIGHT_ARROW
*/
#include "HID-Project.h"
const char BOSS = '.';
const char CONFIRM = 'y';
const char ITEM = 'i';
const char ENTER = 10;
const int BOSS_PAGE_NUM = 14;
const long BOSS_FIRST_X = -30000;
const long BOSS_FIRST_Y = -23500;
const long BOSS_MOVE_X = -15000;
const long BOSS_MOVE_Y = 1500;
const long BOSS_NEXT_PAGE_X = -30000;
const long BOSS_NEXT_PAGE_Y = 4000;
const long BOSS_DISTANCE_Y = 2000;
const int BOSS_COUNTS = 3;
const int BOSS_LIST[BOSS_COUNTS] = {2, 3, 4};
const long LEFT_TOP_ITEM_X = -31500;
const long LEFT_TOP_ITEM_Y = -28000;
const long MAGNUS_OUT_SEAT_X = -26500;
const long MAGNUS_OUT_SEAT_Y = 0;
const long HILLA_OUT_SEAT_X = -25500;
const long HILLA_OUT_SEAT_Y = -2000;
/**
 * 2 = 炎魔
 * 3 = 暴君
 * 4 = 希拉
 * 7 = 拉圖斯
 * 8 ~ 11 = 混4
 * 12 = 凡雷恩
 * 13 = 龍王
 * 14 = 阿卡
 * 17 = 皮卡啾
 * */ 

const char GUIDE = 'u';
const long GUIDE_FIRST_X = -30000;
const long GUIDE_FIRST_Y = -2000;
const long GUIDE_DISTANCE_X = 1650;
const long MOVE_BOUnD_MAX = 32767;
const long MOVE_BOUND_MIN = -32767;

const unsigned long WHEEL_CD = 630;
const char JUMP = 'f';
const char WIND_MOVE = 'w';
const char ELF_SHIELD = 'd';
const char SONG_SKY = 'a';
const char ROPE = 't';

const char FOUNTAIN = 'z';
const unsigned long FOUNTAIN_CD = 58; 
const char TORNADO = 'b';
const unsigned long TORNADO_CD = 20;
const char MONSOON = 'q';
const unsigned long MONSOON_CD = 24;
const char SWIRL = 'e';
const unsigned long SWIRL_CD = 25;
const char FANTASY = ' ';
const unsigned long FANTASY_CD = 8;
const char BIRD = 'v';
const unsigned long BIRD_CD = 25;
const char C2F = 'g';
const unsigned long MONEY_CD = 90; 

const int BUFF_COUNTS = 5;
const char BUFF[5] = {'2', '3', C2F, 'c', 'x'};
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
    if (c == '1') { // daily task
      Serial.println("Input 'a' for 6 tasks, 'b' for 9 tasks");
      WaitInput();
      char next = Serial.read();
      unsigned long BATTLE_TIME[] = {70, 70, 70, 70, 70, 70, 150, 150, 150};
      int GUIDE_DAILY_TASKS = 9; // daily

      if (next == 'a') {
        BATTLE_TIME[4] = 100;
        BATTLE_TIME[5] = 200;
        GUIDE_DAILY_TASKS = 6;
        Serial.println("Input start task (0 ~ 5)");

      } else if (next == 'b') {
        Serial.println("Input start task (0 ~ 8)");

      } else {
        Serial.print("Invalid command: ");
        Serial.println(next);
        return;
      }
      WaitInput();
      int start = (int)Serial.read() - '0';
      if (start < 0 || start >= GUIDE_DAILY_TASKS) {
        Serial.print("Invliad start task number: ");
        Serial.println(start);
        return;
      }
      delay(3000);
      
      for (int i = start; i < GUIDE_DAILY_TASKS; i++) { // daily
        GuideMoving(i+1);
        delay(500);
        Battle(BATTLE_TIME[i], i+1, false, false);
        delay(3000);
      }

    } else if (c == '2') {
      for (int i = 0; i < BOSS_COUNTS; i++) {
        int index = BOSS_LIST[i];
        BossMoving(index);
        delay(2000);
        BossScript(index);
        delay(3000);
      }
      
    } else if (c == '3') {
      int test_index = 7;
      BossMoving(test_index);
      delay(2000);
      Papulatus();
      
    } else if (c == '4') {

    } else if (c == '0') {
      Battle(WHEEL_CD, 0, true, true);
    }
  }
}

void Battle(unsigned long period, int preMove, bool useFountain, bool collectMoney) {
  if (preMove == 1) {
    char toCenterCommand[] = {'d', 'd', 'd'};
    unsigned long minDelay[] = {600, 600, 600};
    unsigned long maxDelay[] = {650, 650, 650};
    Move(toCenterCommand, 3, minDelay, maxDelay);

  } else if (preMove == 2) {
    char toCenterCommand[] = {'d', 'd', 'd', 'd'};
    unsigned long minDelay[] = {600, 600, 600, 600};
    unsigned long maxDelay[] = {650, 650, 650, 650};
    Move(toCenterCommand, 4, minDelay, maxDelay);

  } else if (preMove == 3) {
    char toCenterCommand[] = {'e', 'd', 'd'};
    unsigned long minDelay[] = {1500, 600, 600};
    unsigned long maxDelay[] = {1600, 650, 650};
    Move(toCenterCommand, 3, minDelay, maxDelay);

  } else if (preMove == 4) {
    char toCenterCommand[] = {'s', 's', 's'};
    unsigned long minDelay[] = {600, 600, 600};
    unsigned long maxDelay[] = {650, 650, 650};
    Move(toCenterCommand, 3, minDelay, maxDelay);

  } else if (preMove == 5) {
    char toCenterCommand[] = {'q', 'q', 'q', 'x', 'x', 'x', 'w', 's'};
    unsigned long minDelay[] = {1300, 1100, 1100, 300, 300, 300, 1200, 600};
    unsigned long maxDelay[] = {1400, 1200, 1200, 350, 350, 350, 1250, 650};
    Move(toCenterCommand, 8, minDelay, maxDelay);

  } else if (preMove == 6) {
    char toCenterCommand[] = {'d', 'w', 's', 'z', 'z'};
    unsigned long minDelay[] = {600, 1000, 600, 300, 300};
    unsigned long maxDelay[] = {650, 1200, 650, 350, 350};
    Move(toCenterCommand, 5, minDelay, maxDelay);

  } else if (preMove == 7) {

  } else if (preMove == 8) {
    char toCenterCommand[] = {'q', 's', 's'};
    unsigned long minDelay[] = {1200, 600, 600};
    unsigned long maxDelay[] = {1300, 650, 650};
    Move(toCenterCommand, 3, minDelay, maxDelay);

  } else if (preMove == 9) {
    char toCenterCommand[] = {'z', 'z', 'z', 'w', 's'};
    unsigned long minDelay[] = {300, 300, 300, 1500, 800};
    unsigned long maxDelay[] = {350, 350, 350, 1600, 850};
    Move(toCenterCommand, 5, minDelay, maxDelay);
    collectMoney = true;
  }
  delay(800);

  bool direction = false;
  unsigned long start = millis();
  unsigned long TornadoStart = start;
  unsigned long SwirlStart = start;
  unsigned long MonsoonStart = start;
  unsigned long FantasyStart = start;
  unsigned long FountainStart = start;
  unsigned long BirdStart = start;
  unsigned long MoneyStart = start;

  unsigned long buffStart[5] = {start, start, start, start, start};

  unsigned long time = millis();
  int second = (time-start)/1000;
  bool startUp = true;
  while (second < period) {
    bool underAttack = false;
    SongOfTheSky(direction, 10, 20, 500, 1000);
    direction = !direction;
    
    // Fantasy
    if (startUp || (time-FantasyStart)/1000 > FANTASY_CD) {
      SimpleSkill(direction, FANTASY);
      FantasyStart = millis();
      delay(600);
      Serial.print(second);
      Serial.println("Fantasy");
    }
        
    // Tornado
    time = millis();
    second = (time-start)/1000;
    if (startUp || (time-TornadoStart)/1000 > TORNADO_CD) {
      Tornado(direction);
      TornadoStart = millis();
      delay(random(700, 1000));
      
      Serial.print(second);
      Serial.println("Tornado");
      underAttack = true;
    }

    // Swirl
    time = millis();
    second = (time-start)/1000;
    if (startUp || (time-SwirlStart)/1000 > SWIRL_CD) {
      Swirl(direction);
      SwirlStart = millis();
      delay(random(800, 1000));
      
      Serial.print(second);
      Serial.println("Swirl");
    }

    // Bird
    time = millis();
    second = (time-start)/1000;
    if (startUp || (time-BirdStart)/1000 > BIRD_CD) {
      SimpleSkill(direction, BIRD);
      BirdStart = millis();
      delay(random(800, 1000));
        
      Serial.print(second);
      Serial.println("Bird");
    }

    // Monsoon
    time = millis();
    second = (time-start)/1000;
    if (startUp || (time-MonsoonStart)/1000 > MONSOON_CD) {
      SimpleSkill(direction, MONSOON);
      MonsoonStart = millis();
      delay(random(1500, 1700));
      
      Serial.print(second);
      Serial.println("Monsoon");
    }

    // Buffs
    for (int i = 0; i < BUFF_COUNTS; i++) {
      time = millis();
      second = (time-start)/1000;
      if (startUp || (time-buffStart[i])/1000 > BUFF_CD[i]) {
        SimpleSkill(direction, BUFF[i]);
        buffStart[i] = millis();
        delay(random(800, 1000));
      
        Serial.print(second);
        Serial.println(BUFF_NAME[i]);
      }
    }

    // Fountain
    time = millis();
    second = (time-start)/1000;
    if (useFountain && (startUp || (time-FountainStart)/1000 > FOUNTAIN_CD)) {
      // Move to the specific position
      // MoveToFountain_2_6();
      MoveToFountain_library4();
      
      // Fountain
      Fountain(false);
      FountainStart = millis();
      delay(random(800, 1000));

      Serial.print(second);
      Serial.println("Fountain");
         
      // Move back to origin position
      // BackFromFountain_2_6();
      BackFromFountain_library4();
    }

    // Money
    time = millis();
    second = (time-start)/1000;
    if (startUp || (underAttack && (time-MoneyStart)/1000 > MONEY_CD)) {
      if (collectMoney) {
      // Collect money
      // CollectMoney_2_6();
      // CollectMoney_library4();
      CollectMoney_alley2();
      }
      MoneyStart = millis();
    }
    delay(random(50, 100));
    startUp = false;
  }
}

// ====================== Daily boss ======================
void BossMoving(int index) {
  // Open UI
  Keyboard.write(BOSS);
  delay(1000);

  if (index / BOSS_PAGE_NUM > 0) {
    // Need to go to next page
    AbsoluteMouse.moveTo(BOSS_NEXT_PAGE_X, BOSS_NEXT_PAGE_Y);
    delay(1000);
    AbsoluteMouse.click(MOUSE_LEFT);
    delay(1000);
  }
  // Move mouse to the BOSS location
  AbsoluteMouse.moveTo(BOSS_FIRST_X, BOSS_FIRST_Y + (index % BOSS_PAGE_NUM) * BOSS_DISTANCE_Y);
  delay(500);
  AbsoluteMouse.click(MOUSE_LEFT);
  delay(500);
  AbsoluteMouse.moveTo(BOSS_MOVE_X, BOSS_MOVE_Y);
  delay(500);
  AbsoluteMouse.click(MOUSE_LEFT);
  delay(1000);
}

void BossScript(int index) {
  /**
 * 2 = 炎魔 Zakum
 * 3 = 暴君
 * 4 = 希拉
 * 7 = 拉圖斯
 * 8 ~ 11 = 混4
 * 12 = 凡雷恩
 * 13 = 龍王
 * 14 = 阿卡
 * 17 = 皮卡啾
 * */ 
  switch (index){
  case 2:
    Zakum(10);
    break;

  case 3:
    Magnus(10);
    break;

  case 4:
    Hilla();
    break;

  case 7:
    Papulatus();
    break;
    
  case 8:
    // Pierre();
    break;
    
  case 9:
    // VonBon();
    break;
    
  case 10:
    // Queen();
    break;
    
  case 11:
    // Vellum();
    break;
    
  case 12:
    // VonLeon();
    break;
    
  case 13:
    // Horntail();
    break;
    
  case 14:
    // Arkarium();
    break;
    
  case 17:
    // PinkBean();
    break;
    
  default:
    break;
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

void Zakum(unsigned long period) {
  // Move
  // 1.1 Move to door
  char command1[] = {'d', 'd', 'd', 'd', 'd', 'd', 'd', 'z', 'z'};
  unsigned long minDelay1[] = {550, 550, 550, 550, 550, 550, 550, 800, 445};
  unsigned long maxDelay1[] = {551, 551, 551, 551, 551, 551, 551, 805, 450};
  int counts = 9;
  Move(command1, counts, minDelay1, maxDelay1);
  delay(1000);

  // 1.2 Go into door
  char command2[] = {'w', 's', 'y'};
  unsigned long wait2[] = {700, 700, 1000};
  counts = 3;
  ArrowMove(command2, counts, wait2);
  delay(1000);
    
  // 1.3 Go to boss room
  char command3[] = {'d', 'd'};
  unsigned long minDelay3[] = {550, 550};
  unsigned long maxDelay3[] = {551, 551};
  counts = 2;
  Move(command3, counts, minDelay3, maxDelay3);

  char command4[] = {'y', 'y', 'y', 'y'};
  unsigned long wait4[] = {700, 700, 700, 700};
  counts = 4;
  ArrowMove(command4, counts, wait4);
  delay(2500);

  // Attack
  // 2.1 Throw flame eye out
  WindMove(true, 100, 110);
  delay(500);
  Keyboard.write(ITEM);
  delay(500);
  AbsoluteMouse.moveTo(LEFT_TOP_ITEM_X, LEFT_TOP_ITEM_Y);
  delay(500);
  AbsoluteMouse.click(MOUSE_LEFT);
  delay(500);
  AbsoluteMouse.moveTo(0, 0);
  delay(500);
  AbsoluteMouse.click(MOUSE_LEFT);
  delay(500);
  Keyboard.write(ITEM);
  delay(500);
  
  // 2.2 attack according to period time
  bool direction = true;
  unsigned long start = millis();
  unsigned long time = millis();
  int second = (time - start) / 1000;
  Tornado(true);
  delay(random(800, 1000));
  SimpleSkill(direction, FANTASY);
  delay(600);
  while (second < period) {
    SongOfTheSky(direction, 10, 15, 500, 1000);
    direction = !direction;
    second = (millis() - start) / 1000;
  }

  // Back
  char command5[] = {'d', 'd'};
  unsigned long minDelay5[] = {550, 550};
  unsigned long maxDelay5[] = {551, 551};
  counts = 2;
  Move(command5, counts, minDelay5, maxDelay5);

  char command6[] = {'y', 'd', 'e'};
  unsigned long wait6[] = {500, 500, 500};
  counts = 3;
  ArrowMove(command6, counts, wait6);
  delay(3000);

  char command7[] = {'a', 'x', 'x'};
  unsigned long minDelay7[] = {1000, 200, 200};
  unsigned long maxDelay7[] = {1050, 205, 205};
  counts = 3;
  Move(command7, counts, minDelay7, maxDelay7);
  delay(1000);
    
  char command8[] = {'w'};
  unsigned long wait8[] = {1000};
  counts = 1;
  ArrowMove(command8, counts, wait8);
  delay(3000);
}

void Magnus(unsigned long period) {
  // Move
  // 1.1 Move to door
  char command1[] = {'d', 'e', 'z', 'z'};
  unsigned long minDelay1[] = {700, 800, 445, 445};
  unsigned long maxDelay1[] = {750, 850, 450, 450};
  int counts = 4;
  Move(command1, counts, minDelay1, maxDelay1);
  delay(1000);
  for (int i = 0; i < 5; i++) {
    Turn(true);
    delay(100);
  }
  delay(1000);
  // 1.2 Go into door
  char command2[] = {'w', 's', 'y'};
  unsigned long wait2[] = {800, 800, 1000};
  counts = 3;
  ArrowMove(command2, counts, wait2);
  delay(2500);

  // Attack
  // 2.1 attack according to period time
  char command3[] = {'e', 'e'};
  unsigned long minDelay3[] = {700, 700};
  unsigned long maxDelay3[] = {750, 750};
  counts = 2;
  Move(command3, counts, minDelay3, maxDelay3);
  delay(1000);
  SimpleSkill(true, C2F);
  delay(500);
  bool direction = true;
  unsigned long start = millis();
  unsigned long time = millis();
  int second = (time - start) / 1000;
  Tornado(true);
  delay(random(800, 1000));
  Swirl(true);
  delay(random(800, 1000));
  SimpleSkill(direction, FANTASY);
  delay(600);
  while (second < period) {
    SongOfTheSky(direction, 10, 15, 500, 1000);
    direction = !direction;
    second = (millis() - start) / 1000;
  }

  // Back
  char command4[] = {'e', 'q', 'q', 'q'};
  unsigned long minDelay4[] = {800, 700, 700, 700};
  unsigned long maxDelay4[] = {850, 750, 750, 750};
  counts = 4;
  Move(command4, counts, minDelay4, maxDelay4);
  
  AbsoluteMouse.moveTo(MAGNUS_OUT_SEAT_X, MAGNUS_OUT_SEAT_Y);
  delay(500);
  AbsoluteMouse.click(MOUSE_LEFT);
  delay(500);
  char command5[] = {'d', 'e'};
  unsigned long wait5[] = {700, 700};
  counts = 2;
  ArrowMove(command5, counts, wait5);
  delay(1000);

  char command6[] = {'q', 'q', 'q', 'x'};
  unsigned long minDelay6[] = {700, 700, 700, 300};
  unsigned long maxDelay6[] = {750, 750, 750, 305};
  counts = 4;
  Move(command6, counts, minDelay6, maxDelay6);
    
  char command7[] = {'w'};
  unsigned long wait7[] = {1000};
  counts = 1;
  ArrowMove(command7, counts, wait7);
  delay(3000);
}

void Hilla() {
  // Move
  // 1.1 Move to door
  char command1[] = {'q', 'q', 'd', 'd', 'd', 'd'};
  unsigned long minDelay1[] = {700, 700, 700, 700, 700, 700};
  unsigned long maxDelay1[] = {750, 750, 750, 750, 750, 750};
  int counts = 6;
  Move(command1, counts, minDelay1, maxDelay1);
  delay(1000);

  // 1.2 Go into door
  char command2[] = {'y', 'y', 'y'};
  unsigned long wait2[] = {700, 700, 700};
  counts = 3;
  ArrowMove(command2, counts, wait2);
  delay(2500);

  // Level1
  SimpleSkill(true, FANTASY);
  delay(600);
  WalkingSongSky(60);
  delay(500);
  char command3[] = {'e', 'e', 'z', 'z', 'z'};
  unsigned long minDelay3[] = {700, 700, 200, 200, 200};
  unsigned long maxDelay3[] = {750, 750, 220, 220, 220};
  counts = 5;
  Move(command3, counts, minDelay3, maxDelay3);
  delay(1000);
  Turn(true);
  Turn(true);
  delay(500);

  char command4[] = {'w'};
  unsigned long wait4[] = {500};
  counts = 1;
  ArrowMove(command4, counts, wait4);
  delay(300);
  Turn(false);
  SongOfTheSky(false, 5, 10, 2500, 2600);
  delay(500);
  Turn(true);
  Turn(true);
  ArrowMove(command4, counts, wait4);
  delay(2000);

  // Level2
  SimpleSkill(true, FANTASY);
  delay(600);
  WalkingSongSky(60);
  delay(500);
  counts = 5;
  Move(command3, counts, minDelay3, maxDelay3);
  delay(1000);
  Turn(true);
  Turn(true);
  delay(500);
  
  counts = 1;
  ArrowMove(command4, counts, wait4);
  delay(300);
  Turn(false);
  SongOfTheSky(false, 5, 10, 2500, 2600);
  delay(500);
  Turn(true);
  Turn(true);
  ArrowMove(command4, counts, wait4);
  delay(2000);

  // Boss 
  WalkingSongSky(40);
  delay(500);
  char command5[] = {'e', 'e', 'q', 'q', 'q', 'q', 'q'};
  unsigned long minDelay5[] = {700, 700, 700, 700, 700, 700, 700};
  unsigned long maxDelay5[] = {730, 730, 730, 730, 730, 730, 730};
  counts = 7;
  Move(command5, counts, minDelay5, maxDelay5);
  delay(1000);

  // Back
  AbsoluteMouse.moveTo(HILLA_OUT_SEAT_X, HILLA_OUT_SEAT_Y);
  delay(500);
  AbsoluteMouse.click(MOUSE_LEFT);
  delay(500);
  char command6[] = {'d', 'e'};
  unsigned long wait6[] = {700, 700};
  counts = 2;
  ArrowMove(command6, counts, wait6);
  delay(2500);

  char command7[] = {'q', 'q', 'x'};
  unsigned long minDelay7[] = {700, 700, 250};
  unsigned long maxDelay7[] = {730, 730, 270};
  counts = 3;
  Move(command7, counts, minDelay7, maxDelay7);
  for (int i = 0; i < 5; i++) {
    Turn(true);
    delay(100);
  }
  delay(300);
  ArrowMove(command4, counts, wait4);
  delay(2500);
}

void Papulatus() {
  // Move
  // 1.1 Move to door
  char command1[] = {'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a',
    'd', 'd', 'd', 'd', 'd', 'd', 'x', 'w'};
  unsigned long minDelay1[] = {650, 650, 650, 650, 650, 650,
    650, 650, 650, 650, 650, 650, 650, 650, 300, 1200};
  unsigned long maxDelay1[] = {700, 700, 700, 700, 700, 700,
    700, 700, 700, 700, 700, 700, 700, 700, 320, 1250};
  int counts = 16;
  Move(command1, counts, minDelay1, maxDelay1);
  delay(1000);

  SlightMove(false, 10);

  // 1.2 Go into door
  char command2[] = {'w', 's', 'y', 'y', 'y'};
  unsigned long wait2[] = {700, 700, 700, 700, 700};
  counts = 5;
  ArrowMove(command2, counts, wait2);
  delay(2500);

  // 1.3 Throw the item
  char command3[] = {'d', 'd', 'd', 'd', 'w', 'z'};
  unsigned long minDelay3[] = {650, 650, 650, 650, 1600, 300};
  unsigned long maxDelay3[] = {700, 700, 700, 700, 1650, 320};
  counts = 6;
  Move(command3, counts, minDelay3, maxDelay3);
  delay(1000);
  SlightMove(true, 10);

  Keyboard.write(ITEM);
  delay(500);
  AbsoluteMouse.moveTo(LEFT_TOP_ITEM_X, LEFT_TOP_ITEM_Y);
  delay(500);
  AbsoluteMouse.click(MOUSE_LEFT);
  delay(500);
  AbsoluteMouse.moveTo(0, 0);
  delay(500);
  AbsoluteMouse.click(MOUSE_LEFT);
  delay(500);
  Keyboard.write(ITEM);
  delay(500);
}

void WalkingSongSky(int times) {
  for (int i = 0; i < times; i++) {
    SongOfTheSky(true, 100, 105, 50, 55);
  }
}

// ====================== Daily task ======================
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

void SlightMove(bool direction, int times) {
  for (int i = 0; i < times; i++) {
    Turn(direction);
    delay(100);
  }
  delay(300);
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
    } else if (direction[i] == 'z') {
      WalkLatency(false, 200, 250);
    } else if (direction[i] == 'x') {
      WalkLatency(true, 200, 250);
    }
    delay(random(minDelay[i], maxDelay[i]));
  }
}
/************** Library 4 ***************/
void MoveToFountain_library4() {
  unsigned long rand = random(1);
  if (rand == 0) {
    MoveToFountainA_library4();
  } else if (rand == 1) {

  } else if (rand == 2) {

  } else if (rand == 3) {

  }
}

void MoveToFountainA_library4() {
  char commands[] = {'e', 'd'};
  unsigned long minDelay[] = {400, 750};
  unsigned long maxDelay[] = {410, 850};
  Move(commands, 2, minDelay, maxDelay);
}

void BackFromFountain_library4() {
  char commands[] = {'q', 'a'};
  unsigned long minDelay[] = {400, 900};
  unsigned long maxDelay[] = {410, 1000};
  Move(commands, 2, minDelay, maxDelay);
}

void CollectMoney_alley2() {
  MoveToFountainA_library4();
  delay(800);
  char commands[] = {'s', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'w', 'd', 'd', 'e', 'e'};
  unsigned long minDelay[] = {900, 580, 580, 580, 580, 580, 580, 580, 700, 700, 1000, 650, 650};
  unsigned long maxDelay[] = {1000, 600, 600, 600, 600, 600, 600, 600, 800, 800, 1100, 700, 700};
  Move(commands, 13, minDelay, maxDelay);
}

void CollectMoney_library4() {
  MoveToFountainA_library4();
  delay(800);
  char commands[] = {'s', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'w', 'd', 'd', 'e', 'd'};
  unsigned long minDelay[] = {900, 580, 580, 580, 580, 580, 580, 580, 700, 700, 1000, 400, 900};
  unsigned long maxDelay[] = {1000, 600, 600, 600, 600, 600, 600, 600, 800, 800, 1100, 450, 1000};
  Move(commands, 13, minDelay, maxDelay);
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

/**********************************/
void WaitInput() {
  for (;Serial.available() <= 0;) {
    // Waiting input
  }
}
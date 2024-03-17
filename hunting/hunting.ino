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
const int BOSS_COUNTS = 12;
const int BOSS_LIST[BOSS_COUNTS] = {7, 2, 3, 4, 8, 9, 10, 11, 12, 13, 14, 17};
const long LEFT_TOP_ITEM_X = -31500;
const long LEFT_TOP_ITEM_Y = -28000;
const long MAGNUS_OUT_SEAT_X = -26500;
const long MAGNUS_OUT_SEAT_Y = 0;
const long HILLA_OUT_SEAT_X = -25500;
const long HILLA_OUT_SEAT_Y = -2000;
const long CHAOS4_OUT_X = -30000;
const long CHAOS4_OUT_Y = -2000;
const long CHAOS4_ACTIVATE_X = 0;
const long CHAOS4_ACTIVATE_Y = 0;
const long VONLEON_OUT_X = -22000;
const long VONLEON_OUT_Y = -2000;
/**
 * 7 = 拉圖斯
 * 2 = 炎魔
 * 3 = 暴君
 * 4 = 希拉
 * 8 ~ 11 = 混4
 * 12 = 凡雷恩
 * 13 = 龍王
 * 14 = 阿卡
 * 17 = 皮卡啾
 * */ 

const unsigned long WHEEL_CD = 630;
const char JUMP = 'f';
const char WIND_MOVE = 'w';
const char ELF_SHIELD = 'd';
const char SONG_SKY = 'a';
const char ROPE = '9';
const char ATTACK = '0'; // KEYBOARD_DELETE

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
    if (c == 't') { // stand for testing
      int test_index = 13;
      BossMoving(test_index);
      delay(2000);
      Horntail();

    } else if (c == '2') { // daily boss
      Serial.print("Input start index of boss (0 ~ ");
      Serial.print(BOSS_COUNTS-1);
      Serial.println(")");
      Serial.println("Index map:\n0: Papulatus, 1: Zakum, 2: Magnus, 3: Hilla\n4: Pierre, 5: VonBon, 6: Queen, 7: Vellum\n8: VonLeon, 9: Horntail, 10: Arkarium, 11: PinkBean");
      WaitInput();
      int start = (int)Serial.read() - '0';
      if (start < 0 || start >= BOSS_COUNTS) {
        Serial.print("Invliad start boss index: ");
        Serial.println(start);
        return;
      }
      delay(3000);
      for (int i = start; i < BOSS_COUNTS; i++) {
        int index = BOSS_LIST[i];
        Serial.print("Now is at index: ");
        Serial.println(i);
        BossMoving(index);
        delay(2000);
        BossScript(index);
        delay(3000);
      }
    }
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
    Papulatus(10);
    break;
    
  case 8:
    Pierre();
    break;
    
  case 9:
    VonBon();
    break;
    
  case 10:
    Queen();
    break;
    
  case 11:
    Vellum();
    break;
    
  case 12:
    VonLeon();
    break;
    
  case 13:
    Horntail();
    break;
    
  case 14:
    Arkarium();
    break;
    
  case 17:
    PinkBean();
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

void Zakum(unsigned long period) { // period: second
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
  AbsoluteMouse.moveTo(LEFT_TOP_ITEM_X + 1000, LEFT_TOP_ITEM_Y);
  delay(500);
  AbsoluteMouse.click(MOUSE_LEFT);
  delay(500);
  AbsoluteMouse.moveTo(0, 0);
  delay(500);
  AbsoluteMouse.click(MOUSE_LEFT);
  delay(500);
  Keyboard.write('1');
  delay(500);
  Keyboard.write(ENTER);
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
    SongOfTheSky(true, direction, 10, 15, 500, 1000);
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

void Magnus(unsigned long period) { // period: second
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
    SongOfTheSky(true, direction, 10, 15, 500, 1000);
    direction = !direction;
    second = (millis() - start) / 1000;
  }

  // Back
  char command4[] = {'e', 'e', 'q', 'q', 'q', 'q'};
  unsigned long minDelay4[] = {800, 800, 700, 700, 700, 700};
  unsigned long maxDelay4[] = {850, 850, 750, 750, 750, 750};
  counts = 6;
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
  WalkingSongSky(true, 60);
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
  SongOfTheSky(true, false, 5, 10, 2500, 2600);
  delay(500);
  Turn(true);
  Turn(true);
  ArrowMove(command4, counts, wait4);
  delay(2000);

  // Level2
  SimpleSkill(true, FANTASY);
  delay(600);
  WalkingSongSky(true, 60);
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
  SongOfTheSky(true, false, 5, 10, 2500, 2600);
  delay(500);
  Turn(true);
  Turn(true);
  ArrowMove(command4, counts, wait4);
  delay(2000);

  // Boss 
  WalkingSongSky(true, 40);
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

void Papulatus(unsigned long period) { // period: second
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

  SlightMove(false, 6);

  // 1.2 Go into door
  char command2[] = {'w', 's', 'y', 'y', 'y'};
  unsigned long wait2[] = {700, 700, 700, 700, 700};
  counts = 5;
  ArrowMove(command2, counts, wait2);
  delay(2000);

  // 1.3 Throw the item
  char command3[] = {'d', 'd', 'd', 'd', 'w', 'z'};
  unsigned long minDelay3[] = {650, 650, 650, 650, 1600, 300};
  unsigned long maxDelay3[] = {700, 700, 700, 700, 1650, 320};
  counts = 6;
  Move(command3, counts, minDelay3, maxDelay3);
  delay(1000);
  SlightMove(true, 5);

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
  delay(2000);

  // Attack 
  SlightMove(true, 10);
  Tornado(false);
  delay(random(800, 1000));
  Swirl(false);
  delay(random(800, 1000));
  SimpleSkill(false, FANTASY);
  delay(600);
  SongOfTheSky(true, false, 5, 15, (period-1)*1000, (period+1)*1000);
  delay(1000);

  // Back
  char command4[] = {'s', 'a', 'a', 'a'};
  unsigned long minDelay4[] = {1000, 650, 650, 650};
  unsigned long maxDelay4[] = {1100, 700, 700, 700};
  counts = 4;
  Move(command4, counts, minDelay4, maxDelay4);
  delay(500);

  char command5[] = {'y', 'd', 'e'};
  unsigned long wait5[] = {700, 700, 700};
  counts = 3;
  ArrowMove(command5, counts, wait5);
  delay(2000);
  
  char command6[] = {'a', 'x'};
  unsigned long minDelay6[] = {700, 700};
  unsigned long maxDelay6[] = {750, 750};
  counts = 2;
  Move(command6, counts, minDelay6, maxDelay6);
  
  SlightMove(true, 10);
  delay(300);

  char command7[] = {'w'};
  unsigned long wait7[] = {1000};
  counts = 1;
  ArrowMove(command7, counts, wait7);
  delay(3000);
}

void Pierre() {
    // Move
  // 1.1 Move to door
  char command1[] = {'a'};
  unsigned long minDelay1[] = {650};
  unsigned long maxDelay1[] = {670};
  int counts = 1;
  Move(command1, counts, minDelay1, maxDelay1);
  delay(1000);

  // 1.2 Go into door
  char command2[] = {'y', 'y'};
  unsigned long wait2[] = {700, 700};
  counts = 2;
  ArrowMove(command2, counts, wait2);
  delay(1500);

  // Level1
  SimpleSkill(true, FANTASY);
  delay(600);
  WalkingSongSky(true, 80);
  delay(500);
  char command3[] = {'e', 'a'};
  unsigned long minDelay3[] = {700, 700};
  unsigned long maxDelay3[] = {750, 750};
  counts = 2;
  Move(command3, counts, minDelay3, maxDelay3);
  delay(1000);
  SlightMove(true, 15);

  char command4[] = {'w', 'd', 'e'};
  unsigned long wait4[] = {500, 500, 500};
  counts = 3;
  ArrowMove(command4, counts, wait4);
  delay(1000);

  // Boss
  SimpleSkill(true, FANTASY);
  delay(600);
  SongOfTheSky(true, true, 10, 15, 5000, 5500);
  delay(2000);
  SimpleSkill(true, ATTACK);
  delay(2000);

  // Back
  char command5[] = {'e', 'e', 'q', 'q', 'q', 'q'};
  unsigned long minDelay5[] = {700, 700, 700, 700, 700, 700};
  unsigned long maxDelay5[] = {730, 730, 730, 730, 730, 730};
  counts = 6;
  Move(command5, counts, minDelay5, maxDelay5);
  delay(1000);

  AbsoluteMouse.moveTo(CHAOS4_OUT_X, CHAOS4_OUT_Y);
  delay(500);
  AbsoluteMouse.click(MOUSE_LEFT);
  delay(500);
  char command6[] = {'d', 'e'};
  unsigned long wait6[] = {700, 700};
  counts = 2;
  ArrowMove(command6, counts, wait6);
  delay(2500);
}

void VonBon() {
  // Move
  // 1.1 Move to door
  char command1[] = {'d'};
  unsigned long minDelay1[] = {650};
  unsigned long maxDelay1[] = {670};
  int counts = 1;
  Move(command1, counts, minDelay1, maxDelay1);
  delay(1000);

  // 1.2 Go into door
  char command2[] = {'y', 'y'};
  unsigned long wait2[] = {700, 700};
  counts = 2;
  ArrowMove(command2, counts, wait2);
  delay(1500);

  // Level1
  SimpleSkill(true, FANTASY);
  delay(600);
  WalkingSongSky(true, 80);
  delay(500);
  
  char command3[] = {'e', 'z', 'z', 'w'};
  unsigned long minDelay3[] = {700, 500, 500, 1200};
  unsigned long maxDelay3[] = {750, 520, 520, 1250};
  counts = 4;
  Move(command3, counts, minDelay3, maxDelay3);
  delay(1000);
  SlightMove(false, 6);
      
  char command4[] = {'w', 'd', 'e'};
  unsigned long wait4[] = {500, 500, 500};
  counts = 3;
  ArrowMove(command4, counts, wait4);
  delay(1000);

  // Boss
  AbsoluteMouse.moveTo(CHAOS4_ACTIVATE_X, CHAOS4_ACTIVATE_Y);
  delay(500);
  AbsoluteMouse.click(MOUSE_LEFT);
  delay(500);  
  
  // SimpleSkill(true, FANTASY);
  delay(600);
  SongOfTheSky(true, true, 10, 15, 5000, 5500);
  delay(1000);
  SimpleSkill(true, ATTACK);
  delay(2000);

  // Back
  char command5[] = {'e', 'e', 'q', 'q', 'q', 'q'};
  unsigned long minDelay5[] = {700, 700, 700, 700, 700, 700};
  unsigned long maxDelay5[] = {730, 730, 730, 730, 730, 730};
  counts = 6;
  Move(command5, counts, minDelay5, maxDelay5);
  delay(1000);

  AbsoluteMouse.moveTo(CHAOS4_OUT_X, 0);
  delay(500);
  AbsoluteMouse.click(MOUSE_LEFT);
  delay(500);
  char command6[] = {'d', 'e'};
  unsigned long wait6[] = {700, 700};
  counts = 2;
  ArrowMove(command6, counts, wait6);
  delay(2500);
}

void Queen() {
  // Move
  // 1.1 Move to door
  char command1[] = {'a', 'a'};
  unsigned long minDelay1[] = {650, 650};
  unsigned long maxDelay1[] = {670, 670};
  int counts = 2;
  Move(command1, counts, minDelay1, maxDelay1);
  delay(1000);

  // 1.2 Go into door
  char command2[] = {'y', 'y'};
  unsigned long wait2[] = {700, 700};
  counts = 2;
  ArrowMove(command2, counts, wait2);
  delay(1500);

  // Level1
  SimpleSkill(true, FANTASY);
  delay(600);
  WalkingSongSky(true, 80);
  delay(500);
  
  char command3[] = {'e', 'e', 'a', 'a', 'x'};
  unsigned long minDelay3[] = {700, 700, 700, 700, 500};
  unsigned long maxDelay3[] = {710, 710, 710, 710, 520};
  counts = 5;
  Move(command3, counts, minDelay3, maxDelay3);
  delay(1000);
  SlightMove(true, 10);
      
  char command4[] = {'w', 'd', 'e'};
  unsigned long wait4[] = {500, 500, 500};
  counts = 3;
  ArrowMove(command4, counts, wait4);
  delay(1000);

  // Boss
  AbsoluteMouse.moveTo(CHAOS4_ACTIVATE_X, CHAOS4_ACTIVATE_Y);
  delay(500);
  for (int i = 0; i < 10; i++) {
    AbsoluteMouse.click(MOUSE_LEFT);
    delay(200);  
  }
  
  SimpleSkill(true, FANTASY);
  delay(600);
  SongOfTheSky(true, true, 10, 15, 5000, 5500);
  delay(1000);
  SimpleSkill(true, ATTACK);
  delay(2000);

  // Back
  char command5[] = {'e', 'e', 'q', 'q', 'q', 'q'};
  unsigned long minDelay5[] = {700, 700, 700, 700, 700, 700};
  unsigned long maxDelay5[] = {730, 730, 730, 730, 730, 730};
  counts = 6;
  Move(command5, counts, minDelay5, maxDelay5);
  delay(1000);

  AbsoluteMouse.moveTo(CHAOS4_OUT_X, 0);
  delay(500);
  AbsoluteMouse.click(MOUSE_LEFT);
  delay(500);
  char command6[] = {'d', 'e'};
  unsigned long wait6[] = {700, 700};
  counts = 2;
  ArrowMove(command6, counts, wait6);
  delay(2500);
}

void Vellum() {
  // Move
  // 1.1 Move to door
  char command1[] = {'d', 'd'};
  unsigned long minDelay1[] = {650, 650};
  unsigned long maxDelay1[] = {670, 670};
  int counts = 2;
  Move(command1, counts, minDelay1, maxDelay1);
  delay(1000);

  // 1.2 Go into door
  char command2[] = {'y', 'y'};
  unsigned long wait2[] = {700, 700};
  counts = 2;
  ArrowMove(command2, counts, wait2);
  delay(1500);

  // Level1
  SimpleSkill(true, FANTASY);
  delay(600);
  WalkingSongSky(true, 80);
  delay(500);
  
  char command3[] = {'e', 'q'};
  unsigned long minDelay3[] = {800, 700};
  unsigned long maxDelay3[] = {810, 710};
  counts = 2;
  Move(command3, counts, minDelay3, maxDelay3);
  delay(1000);
  SlightMove(true, 8);

  char command4[] = {'w', 'd', 'e'};
  unsigned long wait4[] = {500, 500, 500};
  counts = 3;
  ArrowMove(command4, counts, wait4);
  delay(1000);

  // Boss
  AbsoluteMouse.moveTo(CHAOS4_ACTIVATE_X + 11000, CHAOS4_ACTIVATE_Y);
  delay(500);
  AbsoluteMouse.click(MOUSE_LEFT);
  delay(500);  
  
  SimpleSkill(true, FANTASY);
  delay(600);
  SongOfTheSky(true, true, 10, 15, 6000, 6500);
  delay(1000);
  SimpleSkill(true, ATTACK);
  delay(3000);

  // Back
  char command5[] = {'e', 'e', 'q', 'q', 'q', 'q'};
  unsigned long minDelay5[] = {700, 700, 700, 700, 700, 700};
  unsigned long maxDelay5[] = {730, 730, 730, 730, 730, 730};
  counts = 6;
  Move(command5, counts, minDelay5, maxDelay5);
  delay(1000);

  AbsoluteMouse.moveTo(CHAOS4_OUT_X + 2000, 0);
  delay(500);
  AbsoluteMouse.click(MOUSE_LEFT);
  delay(500);
  char command6[] = {'d', 'e'};
  unsigned long wait6[] = {700, 700};
  counts = 2;
  ArrowMove(command6, counts, wait6);
  delay(2500);
}

void VonLeon() {
  // Move
  // 1.1 Move to door
  char command1[] = {'d', 'd', 'd', 'x'};
  unsigned long minDelay1[] = {650, 650, 650, 300};
  unsigned long maxDelay1[] = {655, 655, 655, 310};
  int counts = 4;
  Move(command1, counts, minDelay1, maxDelay1);
  delay(300);
  SlightMove(false, 5);
  delay(500);

  // 1.2 Go into door
  char command2[] = {'w', 'y', 's', 's', 'y'};
  unsigned long wait2[] = {700, 700, 700, 700, 700};
  counts = 5;
  ArrowMove(command2, counts, wait2);
  delay(1500);

  // Boss
  char command3[] = {'q', 'q', 'e', 'e', 'e', 'e', 'e'};
  unsigned long minDelay3[] = {700, 700, 700, 700, 700, 700, 700};
  unsigned long maxDelay3[] = {710, 710, 710, 710, 710, 710, 710};
  counts = 7;
  Move(command3, counts, minDelay3, maxDelay3);
  delay(500);
      
  char command4[] = {'y', 'd', 'e'};
  unsigned long wait4[] = {500, 500, 500};
  counts = 3;
  ArrowMove(command4, counts, wait4);
  delay(1000);

  // Boss
  SimpleSkill(true, FANTASY);
  delay(600);
  SongOfTheSky(true, true, 10, 15, 7000, 7500);
  delay(1000);
  SimpleSkill(true, ATTACK);
  delay(2000);

  // Back
  char command5[] = {'q', 'q', 'q', 'q', 'q', 'q'};
  unsigned long minDelay5[] = {700, 700, 700, 700, 700, 700};
  unsigned long maxDelay5[] = {730, 730, 730, 730, 730, 730};
  counts = 6;
  Move(command5, counts, minDelay5, maxDelay5);
  delay(1000);

  AbsoluteMouse.moveTo(VONLEON_OUT_X, VONLEON_OUT_Y);
  delay(500);
  AbsoluteMouse.click(MOUSE_LEFT);
  delay(500);
  char command6[] = {'d', 'e'};
  unsigned long wait6[] = {700, 700};
  counts = 2;
  ArrowMove(command6, counts, wait6);
  delay(2000);

  char command7[] = {'z'};
  unsigned long minDelay7[] = {700};
  unsigned long maxDelay7[] = {730};
  counts = 1;
  Move(command7, counts, minDelay7, maxDelay7);
  delay(500);

  char command8[] = {'w'};
  unsigned long wait8[] = {700};
  counts = 1;
  ArrowMove(command8, counts, wait8);
  delay(2500);
}

void Horntail() {
  // Move
  // 1.1 Move to door
  char command1[] = {'e', 's', 's', 's'};
  unsigned long minDelay1[] = {700, 650, 650, 650};
  unsigned long maxDelay1[] = {710, 655, 655, 655};
  int counts = 4;
  Move(command1, counts, minDelay1, maxDelay1);
  delay(800);

  // 1.2 Go into door
  char command2[] = {'y'};
  unsigned long wait2[] = {700};
  counts = 1;
  ArrowMove(command2, counts, wait2);
  delay(2000);

  char command3[] = {'d', 'd', 'd'};
  unsigned long minDelay3[] = {700, 700, 700};
  unsigned long maxDelay3[] = {710, 710, 710};
  counts = 3;
  Move(command3, counts, minDelay3, maxDelay3);
  delay(500);

  char command4[] = {'y', 'd', 'e'};
  unsigned long wait4[] = {500, 500, 500};
  counts = 3;
  ArrowMove(command4, counts, wait4);
  delay(1000);

  counts = 3;
  Move(command3, counts, minDelay3, maxDelay3);
  delay(500);

  char command5[] = {'y', 'y', 's', 's', 'e'};
  unsigned long wait5[] = {500, 500, 500, 500, 500};
  counts = 5;
  ArrowMove(command5, counts, wait5);
  delay(1000);

  char up[] = {'w'};
  unsigned long waitUp[] = {500};
  SimpleSkill(true, FANTASY);
  delay(800);
  char command6[] = {'d', 'e', 'z', 'z', 'z'};
  unsigned long minDelay6[] = {700, 700, 2000, 300, 300};
  unsigned long maxDelay6[] = {710, 710, 2050, 310, 310};
  counts = 5;
  Move(command6, counts, minDelay6, maxDelay6);
  delay(500);
  SlightMove(true, 3);
  delay(300);
  SimpleSkill(true, ROPE);
  delay(1500);
  ArrowMove(up, 1, waitUp);
  delay(1000);

  SimpleSkill(true, FANTASY);
  delay(800);
  char command7[] = {'a', 'q', 'x', 'x', 'x'};
  Move(command7, counts, minDelay6, minDelay6);
  delay(500);
  SlightMove(false, 3);
  delay(300);
  SimpleSkill(false, ROPE);
  delay(1500);
  ArrowMove(up, 1, waitUp);
  delay(1200);

  char command8[] = {'e', 'w', 'x', 'x'};
  unsigned long minDelay8[] = {700, 1500, 300, 300};
  unsigned long maxDelay8[] = {705, 1550, 310, 310};
  counts = 4;
  Move(command8, counts, minDelay8, maxDelay8);
  delay(1000);
  for (int i = 0; i < 32; i++) {
    SongOfTheSky(false, true, 5, 8, 30, 35);
    delay(200);
  }
  
  // ATTACK
  delay(4000);
  SimpleSkill(true, FANTASY);
  delay(800);
  Tornado(false);
  delay(6000);
      
  // BACK
  char command9[] = {'q'};
  unsigned long minDelay9[] = {2000};
  unsigned long maxDelay9[] = {2050};
  Move(command9, 1, minDelay9, maxDelay9);
  delay(500);

  char command10[] = {'y', 'd', 'e'};
  unsigned long wait10[] = {500, 500, 500};
  counts = 3;
  ArrowMove(command10, counts, wait10);
  delay(2200);

  ArrowMove(command10, counts, wait10);
  delay(2200);

  char single[] = {'z'};
  unsigned long minSingle[] = {500};
  unsigned long maxSingle[] = {550};
  Move(single, 1, minSingle, maxSingle);
  delay(600);
  SlightMove(false, 5);
  ArrowMove(up, 1, waitUp);
  delay(2000);
  ArrowMove(up, 1, waitUp);
  delay(2000);
}

void Arkarium() {

}

void PinkBean() {

}

// ====================== Commands ======================
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
unsigned long SongOfTheSky(bool walk, bool direction, unsigned long minWalk, unsigned long maxWalk, unsigned long minDuration, unsigned long maxDuration) {
  if (walk) {
    if (direction == true) {
      Keyboard.press(KEY_RIGHT_ARROW);
    } else {
      Keyboard.press(KEY_LEFT_ARROW);
    }
  }
  unsigned long distance = random(minWalk, maxWalk);
  delay(distance);
  Keyboard.press(SONG_SKY);
  delay(random(minDuration, maxDuration));
  Keyboard.releaseAll();
  return distance;
}

void WalkingSongSky(bool direction, int times) {
  for (int i = 0; i < times; i++) {
    SongOfTheSky(true, direction, 100, 105, 50, 55);
  }
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
    } else if (direction[i] == 'r') {
      SimpleSkill(true, ROPE);
    }
    delay(random(minDelay[i], maxDelay[i]));
  }
}

void WaitInput() {
  for (;Serial.available() <= 0;) {
    // Waiting input
  }
}
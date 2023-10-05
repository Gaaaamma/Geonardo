/*
Command list:
s(115): single click

d(100): double click

m(109): move mouse position (with two following integer separated by ',' end with '\n' or any character)
  - x: absolute x moving
  - y: absolute y moving
  - Example: m240,31\n, m-243,-43.

r(114): move mouse relatively (with two following integer separated by ',' end with '\n' or any character)
  - x: relative x moving
  - y: relative y moving
  - Example: r240,31\n, r-243,-43.

c(99): move mouse position to center (0,0)

g(103): get current cursor position (x,y)

e(101): keyboard write 'Enter'

k(107): key in a certain character (with one following ASCII code)

l(108): l means lab is for experiment => MoveLikeHuman(long x, long y)
  - x: relative x moving
  - y: relative y moving
  - Example: l134,-775\n, l-1001,324.
*/
#include "HID-Project.h"

long CLICK_FREQUENCY = 100;
const char ENTER = 13;

int currentX = 0;
int currentY = 0;

const long MOVE_BOUND_MAX = 32767;
const long MOVE_BOUND_MIN = -32767;


/*               HID AbsoluteMouse coordination 
(-32767, -32767) ------------- # ------------- (32767, -32767)
                 |             |             |
                 |             |             |
                 |------------ 0 ------------| (32767, 0)
                 |             |             |
                 |             |             |
(-32767, 32767)  ------------- # ------------- (32767, 32767)
*/

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  randomSeed(analogRead(0));
}

void loop() {
  if (Serial.available() > 0) {
    // Start with a command character
    char c = (char)Serial.read();

    if (c == 'm') {  // Move to a specific position (x, y)
      long x = Serial.parseInt();
      Serial.read();  // consume ',' in the buffer
      long y = Serial.parseInt();

      // Check valid movement
      if ((x <= MOVE_BOUND_MAX && x >= MOVE_BOUND_MIN) && (y <= MOVE_BOUND_MAX && y >= MOVE_BOUND_MIN)) {
        AbsoluteMouse.moveTo((int)x, (int)y);
        updateLocation((int)x, (int)y);
        getLocation();

      } else {
        Serial.println("[Error] invalid move");
      }

    } else if (c == 'r') { // Move relatively
      long x = Serial.parseInt();
      Serial.read();  // consume ',' in the buffer
      long y = Serial.parseInt();

      long newX = (long)currentX + x;
      long newY = (long)currentY + y;
      
      // Check if this relatively move will exceed the bound
      if ((newX <= MOVE_BOUND_MAX && newX >= MOVE_BOUND_MIN) && (newY <= MOVE_BOUND_MAX && newY >= MOVE_BOUND_MIN)) {
        AbsoluteMouse.move((int)x, (int)y);
        currentX = (int)newX;
        currentY = (int)newY;
        getLocation();

      } else {
        if (newX > MOVE_BOUND_MAX) {
          newX = MOVE_BOUND_MAX;
        } else if (newX < MOVE_BOUND_MIN) {
          newX = MOVE_BOUND_MIN;
        }

        if (newY > MOVE_BOUND_MAX) {
          newY = MOVE_BOUND_MAX;
        } else if (newY < MOVE_BOUND_MIN) {
          newY = MOVE_BOUND_MIN;
        }
        AbsoluteMouse.moveTo((int)newX, (int)newY);
        updateLocation((int)newX, (int)newY);
        getLocation();

      }

    } else if (c == 'c') {  // Move to the center of the screen
      AbsoluteMouse.moveTo(0, 0);
      updateLocation(0, 0);
      getLocation();

    } else if (c == 'g') {  // Get current location
      getLocation();

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

    } else if (c == 'l') {
      // Experiment commnad

    } else {
      Serial.println("[Error] Invalid command");
    }
    CleanBuffer();
  }
}

void SafeMove(signed char x, signed char y) {
  String message = "[Safe Move] move (";
  message += String(x);
  message += ",";
  message += String(y);
  message += ")";
  Serial.println(message);
}

void MoveLikeHuman(int targetX, int targetY) {
}

void Click(uint8_t b) {
  // Must use a switch or some controller to keep it safe
  AbsoluteMouse.click(b);
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

void updateLocation(int x, int y) {
  currentX = x;
  currentY = y;
}

void getLocation() {
  String message = "[Location] cursor(";
  message += String(currentX);
  message += ",";
  message += String(currentY);
  message += ")";
  Serial.println(message);
}
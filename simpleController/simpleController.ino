/*
Command list:
s: single click
d: double click
m: move mouse position (with two following integer)
  - x: relative x moving
  - y: relative y moving
e: keyboard write 'Enter'
k: key in a certain character (with one following ASCII code)
*/

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available()) {
    while (Serial.available()) {
      int c = Serial.read();
      Serial.print(char(c));
    }
  }
}
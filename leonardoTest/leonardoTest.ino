void setup() {
  Serial.begin(9600);
}

void loop() {
  while (Serial.available() > 0) {
    int data = Serial.read();
    Serial.print(char(data));
  }
}
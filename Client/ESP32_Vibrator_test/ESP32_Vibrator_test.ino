int motorPin = 15;

void setup() {

  pinMode(motorPin, OUTPUT);

}

void loop() {
  digitalWrite(motorPin, HIGH);
  delay(200);
  digitalWrite(motorPin, LOW);
  delay(200);
  digitalWrite(motorPin, HIGH);
  delay(200);
  digitalWrite(motorPin, LOW);
  delay(500);
}

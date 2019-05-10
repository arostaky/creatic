int motorPin = 8;
 
void setup() {
 
  pinMode(motorPin, OUTPUT);
 
}
 
void loop() {
 
  digitalWrite(motorPin, 100);
  delay(1000);
  digitalWrite(motorPin, 200);
  delay(1000);
  digitalWrite(motorPin, LOW);
  delay(1000);
 
}

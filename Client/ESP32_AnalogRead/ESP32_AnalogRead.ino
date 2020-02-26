/*
  AnalogReadSerial ESP32 ADC example

  Reads an analog input on ADC1_0, prints the result to the Serial Monitor.
  Graphical representation is available using Serial Plotter (Tools > Serial Plotter menu).
  Attach the center pin of a potentiometer to pin A0, and the outside pins to +3.3V and ground.

  https://circuits4you.com
*/
int vibrator = 36;
// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  pinMode(vibrator, OUTPUT);
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog ADC1_0:
  int sensorValue = analogRead(A6);
  int sensorValueB = analogRead(A7);
  int sensorValueC = analogRead(A4);
  int sensorValueD = analogRead(A5);
  
  // print out the value you read:
  Serial.print("Sensor0: " );
  Serial.print(sensorValue);
  Serial.println(" ");
  Serial.print("Sensor1: " );
  Serial.print(sensorValueB);
  Serial.println(" ");
  Serial.print("Sensor2: " );
  Serial.print(sensorValueC);
  Serial.println(" ");
  Serial.print("Sensor3: " );
  Serial.print(sensorValueD);
  Serial.println(" ");
  digitalWrite(36, HIGH);
  delay(1500);        // delay in between reads for stability
}

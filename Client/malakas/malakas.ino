//fastLed lib:
#define FASTLED_ESP32_I2S
#include <FastLED.h>

#include <Wire.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>
#include <OSCBundle.h>
#include <OSCData.h>
#ifndef STASSID
#define STASSID "TPCast_AP2G"
#define STAPSK  "12345678"
#endif
// Set your Static IP address
IPAddress local_IP(192, 168, 144, 102);
// Set your Gateway IP address
IPAddress gateway(192, 168, 144, 1);
IPAddress subnet(255, 255, 0, 0);

WiFiUDP UDP;

// OSC and wifi:
const char* ssid     = STASSID;
const char* password = STAPSK;

const char* host = "192.168.144.100";
//const uint16_t port = 12000;
const IPAddress outIp(192, 168, 144, 100);       // remote IP (not needed for receive)
const unsigned int outPort = 13000;          // remote port (not needed for receive)
const unsigned int localPort = 10000;

// MPU6050 Slave Device Address
const uint8_t MPU6050SlaveAddress = 0x68;

// Select SDA and SCL pins for I2C communication
const uint8_t scl = 25; // GPIO 25
const uint8_t sda = 26; // GPIO 26

// sensitivity scale factor respective to full scale setting provided in datasheet
const uint16_t AccelScaleFactor = 16384;
const uint16_t GyroScaleFactor = 131;

// MPU6050 few configuration register addresses
const uint8_t MPU6050_REGISTER_SMPLRT_DIV   =  0x19;
const uint8_t MPU6050_REGISTER_USER_CTRL    =  0x6A;
const uint8_t MPU6050_REGISTER_PWR_MGMT_1   =  0x6B;
const uint8_t MPU6050_REGISTER_PWR_MGMT_2   =  0x6C;
const uint8_t MPU6050_REGISTER_CONFIG       =  0x1A;
const uint8_t MPU6050_REGISTER_GYRO_CONFIG  =  0x1B;
const uint8_t MPU6050_REGISTER_ACCEL_CONFIG =  0x1C;
const uint8_t MPU6050_REGISTER_FIFO_EN      =  0x23;
const uint8_t MPU6050_REGISTER_INT_ENABLE   =  0x38;
const uint8_t MPU6050_REGISTER_ACCEL_XOUT_H =  0x3B;
const uint8_t MPU6050_REGISTER_SIGNAL_PATH_RESET  = 0x68;

int16_t AccelX, AccelY, AccelZ, Temperature, GyroX, GyroY, GyroZ;
int sensorA, sensorB, sensorC, sensorD;
int lightSensor;
// create global variables for motor and delay function without delay() global from arduino
int motorPin = 15;
// Variables will hange:
int motorState = LOW;             // motorState used to set the motor
// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long previousMillis = 0;        // will store last time motor was updated
// constants won't change:
const long interval = 1000;
unsigned long countMotor = 0;
long OnTime = 20;           // milliseconds of on-time
long OffTime = 50;          // milliseconds of off-time
// How many leds in your strip?
#define NUM_LEDS 2
#define LED_PIN 2
unsigned long countLed = 0;
unsigned long previousMillisB = 0;
long OnTimeB = 33;           // milliseconds of on-time
long OffTimeB = 1000;          // milliseconds of off-time
// Define the array of leds
CRGB leds[NUM_LEDS];
//OSC get:
OSCErrorCode error;
#define MSG_HEADER   "/Flicker"
int ReadFlickering = 1;
void setup() {
  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);
  Serial.begin(9600);
  Wire.begin(sda, scl);
  MPU6050_Init();
  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
      // Configures static IP address
  if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("STA Failed to configure");
  }
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  //Launche UDP Connection
  LaunchUDP(localPort);
  //motor vibration:
  pinMode(motorPin, OUTPUT);
  pinMode(27, INPUT);
}

void loop() {
  ledMethod(ReadFlickering);
  double Ax, Ay, Az, T, Gx, Gy, Gz;

  Read_RawValue(MPU6050SlaveAddress, MPU6050_REGISTER_ACCEL_XOUT_H);

  //divide each with their sensitivity scale factor
  Ax = (double)AccelX / AccelScaleFactor;
  Ay = (double)AccelY / AccelScaleFactor;
  Az = (double)AccelZ / AccelScaleFactor;
  T =  (double)Temperature / 340 + 36.53; //temperature formula
  Gx = (double)GyroX / GyroScaleFactor;
  Gy = (double)GyroY / GyroScaleFactor;
  Gz = (double)GyroZ / GyroScaleFactor;
  //
//  Serial.print("Ax: "); Serial.print(Ax);
//  Serial.print(" Ay: "); Serial.print(Ay);
//  Serial.print(" Az: "); Serial.print(Az);
//  Serial.print(" T: "); Serial.print(T);
//  Serial.print(" Gx: "); Serial.print(Gx);
//  Serial.print(" Gy: "); Serial.print(Gy);
//  Serial.print(" Gz: "); Serial.println(Gz);
  // sensor read:
  sensorRead();
  // light sensor:
  lightSensorRead();
  //Send OSC Message
  // motorVibration:
  motorVibrator();

//OSC Handle
  OSCMessage msgIn;
  int size = UDP.parsePacket();

  if (size > 0) {
    while (size--) {
      msgIn.fill(UDP.read());
    }
    if (!msgIn.hasError()) {
      msgIn.dispatch(MSG_HEADER, ReadFlickMsg );
    }
      else {
      error = msgIn.getError();
      Serial.print("error: ");
      Serial.println(error);
    }
  }
  OSCBundle bndl;
  UDP.beginPacket(outIp, outPort);
  OSCMessage msg("/Yorgos");
  msg.add(Ax);
  msg.add(Ay);
  msg.add(Az);
  msg.add(T);
  msg.add(Gx);
  msg.add(Gy);
  msg.add(Gz);
  //add sensors msg:
  msg.add(sensorA);
  msg.add(sensorB);
  msg.add(sensorC);
  msg.add(sensorD);
  msg.add(lightSensor);
  msg.send(UDP);
  UDP.endPacket();
  msg.empty();
  delay(10);
}

void I2C_Write(uint8_t deviceAddress, uint8_t regAddress, uint8_t data) {
  Wire.beginTransmission(deviceAddress);
  Wire.write(regAddress);
  Wire.write(data);
  Wire.endTransmission();
}

// read all 14 register
void Read_RawValue(uint8_t deviceAddress, uint8_t regAddress) {
  Wire.beginTransmission(deviceAddress);
  Wire.write(regAddress);
  Wire.endTransmission();
  Wire.requestFrom(deviceAddress, (uint8_t)14);
  AccelX = (((int16_t)Wire.read() << 8) | Wire.read());
  AccelY = (((int16_t)Wire.read() << 8) | Wire.read());
  AccelZ = (((int16_t)Wire.read() << 8) | Wire.read());
  Temperature = (((int16_t)Wire.read() << 8) | Wire.read());
  GyroX = (((int16_t)Wire.read() << 8) | Wire.read());
  GyroY = (((int16_t)Wire.read() << 8) | Wire.read());
  GyroZ = (((int16_t)Wire.read() << 8) | Wire.read());
}

//configure MPU6050
void MPU6050_Init() {
  delay(150);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_SMPLRT_DIV, 0x07);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_PWR_MGMT_1, 0x01);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_PWR_MGMT_2, 0x00);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_CONFIG, 0x00);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_GYRO_CONFIG, 0x00);//set +/-250 degree/second full scale
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_ACCEL_CONFIG, 0x00);// set +/- 2g full scale
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_FIFO_EN, 0x00);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_INT_ENABLE, 0x01);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_SIGNAL_PATH_RESET, 0x00);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_USER_CTRL, 0x00);
}
// Launch UDP – returns true if successful or false if not
boolean LaunchUDP(unsigned int localPort) {
  boolean state = false;
  Serial.println("");
  Serial.println("Connecting to UDP");
  if (UDP.begin(localPort) == 1) {
    Serial.println("Connection successful");
    state = true;
  }
  else {
    Serial.println("Connection failed");
  }
  return state;
}
void sensorRead() {
  sensorA = analogRead(A6);
  sensorB = analogRead(A7);
  sensorC = analogRead(A4);
  sensorD = analogRead(A5);
  // print out the value you read:
//  Serial.print("Sensor0: " );
//  Serial.print(sensorA);
//  Serial.println(" ");
//  Serial.print("Sensor1: " );
//  Serial.print(sensorB);
//  Serial.println(" ");
//  Serial.print("Sensor2: " );
//  Serial.print(sensorC);
//  Serial.println(" ");
//  Serial.print("Sensor3: " );
//  Serial.print(sensorD);
//  Serial.println(" ");
  //return [sensorA, sensorB, sensorC, sensorD];
  delay(10);

}
void lightSensorRead() {
  lightSensor = analogRead(A0);
//  Serial.println("Light sensor:");
//  Serial.println("");
//  Serial.println(lightSensor);
//  Serial.println(" ");
  delay(10);

}
void motorVibrator() {
  unsigned long currentMillis = millis();
  countMotor++;
  if ((motorState == HIGH) && (currentMillis - previousMillis >= OnTime))
  {
    motorState = LOW;  // Turn it off
    previousMillis = currentMillis;  // Remember the time
    digitalWrite(motorPin, motorState);  // Update the actual motor
  }
  else if ((motorState == LOW) && (currentMillis - previousMillis >= OffTime))
  {
    motorState = HIGH;  // turn it on
    previousMillis = currentMillis;   // Remember the time
    digitalWrite(motorPin, motorState);   // Update the actual motor
  }
  if (countMotor == 4) {
    // OnTime = 0;
    OffTime = 1100;
  }
  if (countMotor == 5) {
    //OnTime = 20;
    OffTime = 20;
    countMotor = 0;
  }
}
void ledMethod(bool param) {
  unsigned long currentMillisB = millis();
  countLed++;
  if ((param == 0 )&&(currentMillisB - previousMillisB >= OnTimeB)) {
    FastLED.show();
    leds[0] = CHSV(random8(255), 255, 255);
    leds[1] = CHSV(random8(255), 255, 255);
  } else if ((param == 1)&&(currentMillisB - previousMillisB >= OffTimeB)) {
    FastLED.show();
    leds[0] = CRGB::Blue;
    leds[1] = CRGB::Blue;
  }
}
//OSC CALLBACK
void ReadFlickMsg (OSCMessage &msg) {
  ReadFlickering = msg.getInt(0);
  //ledMethod(ReadFlickering);
  //verbos
  Serial.print("Reading flickering....");
  //Serial.print(":");
  Serial.println(ReadFlickering);
}

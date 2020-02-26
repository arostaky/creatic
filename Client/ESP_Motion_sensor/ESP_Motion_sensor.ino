/*
    This sketch establishes a TCP connection to a "quote of the day" service.
    It sends a "hello" message, and then prints received data.
*/

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>
#include <OSCBundle.h>
#include <OSCData.h>
#ifndef STASSID
#define STASSID "Patricio"
#define STAPSK  "Patricio123"
#endif
WiFiUDP UDP;
// utlrasonic pinout
#define ULTRASONIC_TRIG_PIN     5   // pin TRIG to D1
#define ULTRASONIC_ECHO_PIN     4   // pin ECHO to D2
// rgb led:
#define redPin 14
#define greenPin 12
#define bluePin 13
//wifi:
const char* ssid     = STASSID;
const char* password = STAPSK;

const char* host = "10.42.0.1";
//const uint16_t port = 12000;
const IPAddress outIp(10,42,0,1);          // remote IP (not needed for receive)
const unsigned int outPort = 14000;          // remote port (not needed for receive)
const unsigned int localPort = 10000; 
OSCErrorCode error;
unsigned int ledState = LOW;              // LOW means led is *on*
void setup() {
  // ultraonic setup 
  pinMode(ULTRASONIC_TRIG_PIN, OUTPUT);
  pinMode(ULTRASONIC_ECHO_PIN, INPUT);
  //rgb pin:
  pinMode(14,OUTPUT);
  pinMode(13,OUTPUT);
  pinMode(12,OUTPUT);
  Serial.begin(115200);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
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
}

void loop() {
  //digitalWrite(greenPin,HIGH);
  //delayMicroseconds(10); 
  long duration;
  int distance;
  digitalWrite(ULTRASONIC_TRIG_PIN, LOW);  
  delayMicroseconds(2); 
  
  digitalWrite(ULTRASONIC_TRIG_PIN, HIGH);
  delayMicroseconds(10);

  
  digitalWrite(ULTRASONIC_TRIG_PIN, LOW);
  duration = pulseIn(ULTRASONIC_ECHO_PIN, HIGH);
  distance = (duration/2) / 29.1;
 // Serial.print("********** Ultrasonic Distance: ");
 // Serial.print(distance);
 // Serial.println(" cm");
  //Print Values
  //Serial.println(Value);
  //Serial.println(Time);
  //Receive OSC Message:
    OSCMessage getmsg;
    int size = UDP.parsePacket();

  if (size > 0) {
    while (size--) {
      getmsg.fill(UDP.read());
    }
    if (!getmsg.hasError()) {
      getmsg.dispatch("/led", led);
      Serial.println("led");
    } else {
      error = getmsg.getError();
      Serial.print("error: ");
      Serial.println(error);
    }
  }
  //Send OSC Message
  OSCBundle bndl;
  UDP.beginPacket(outIp, outPort);
  OSCMessage msg("/Ultra");
  msg.add(distance); 
  msg.send(UDP);
  UDP.endPacket();
  msg.empty();
  delay(100);
  
  
}
// Launch UDP – returns true if successful or false if not
boolean LaunchUDP(unsigned int localPort){
  boolean state = false;
  Serial.println("");
  Serial.println("Connecting to UDP");
  if(UDP.begin(localPort) == 1){
    Serial.println("Connection successful");
    state = true;
  }
  else{
    Serial.println("Connection failed");
  }
  return state;
}
void led(OSCMessage &msg) {
  ledState = msg.getInt(0);
  digitalWrite(bluePin, ledState);
  Serial.print("/led: ");
  Serial.println(ledState);
}



#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>
#include <OSCBundle.h>
#include <OSCData.h>


#define LEFT_PIN      D1  // pin for Left Hand vibrator 
#define RIGHT_PIN     D2  // Pin for Right Hand vibrator 
#define HAND_PIN      A0  // Pin for Conductivity 
#define LEFT_HEADER   "/LeftVibe"
#define RIGHT_HEADER  "/RightVibe"
#define CONDUCTIVE_HEADER   "/ConductHand"
//Wifi connection Adress
const char *ssid = "TPCast_AP2G";
const char *password = "12345678";
// Set your Static IP address
IPAddress local_IP(192, 168, 144, 103);
// Set your Gateway IP address
IPAddress gateway(192, 168, 144, 1);
IPAddress subnet(255, 255, 0, 0);
WiFiUDP UDP;
const IPAddress outIp(192,168,144,100);        // remote IP (not needed for receive)
const unsigned int outPort = 14000;          // remote port (not needed for receive)
const unsigned int localPort = 10000;   

//OSC
OSCErrorCode error;

void setup() {

  //Serial DEBUG
  Serial.begin(115200);

  //WIFI Init
  
  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
  // Configures static IP address
  if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("STA Failed to configure");
  }
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

    int test=0;
  while (WiFi.status() != WL_CONNECTED  && test <= 20) {
    delay(500);
    Serial.print(".");
    test++;
  }

  
  Serial.println();
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  //Launche UDP Connection
  LaunchUDP(localPort);
  
}

void loop() {

  //OSC Handle
  OSCMessage msg;
  int size = UDP.parsePacket();

  if (size > 0) {
    while (size--) {
      msg.fill(UDP.read());
    }
    if (!msg.hasError()) {
      msg.dispatch(LEFT_HEADER, LeftVibe );
      msg.dispatch(RIGHT_HEADER, RightVibe );
    }
      else {
      error = msg.getError();
      Serial.print("error: ");
      Serial.println(error);
    }
  }
  
  //Get ConductValue
  int ConductValue = analogRead(A0);
  int ConductTime = millis();  
  /*
  //Print Values
  Serial.print (CONDUCTIVE_HEADER);
  Serial.print (ConductTime);
  Serial.print (":");
  Serial.println(ConductValue);*/
  
  //Send OSC Message
  OSCBundle bndl;
  UDP.beginPacket(outIp, outPort);
  OSCMessage msgOut(CONDUCTIVE_HEADER);
  msgOut.add(ConductValue);
  msgOut.add(ConductTime); 
  msgOut.send(UDP);
  UDP.endPacket();
  msgOut.empty();
  delay(50);
}

//OSC CALLBACK
void LeftVibe (OSCMessage &msg) {
  int ReadLeftVibeSpeed = msg.getInt(0);
  int LeftVibeSpeed = map(ReadLeftVibeSpeed,100,0,0,1024);
  analogWrite(LEFT_PIN, LeftVibeSpeed);

  //verbos
  Serial.print(LEFT_HEADER);
  Serial.print(":");
  Serial.println(LeftVibeSpeed);
}

//OSC CALLBACK
void RightVibe (OSCMessage &msg) {
  int ReadRightVibeSpeed = msg.getInt(0);
  int RightVibeSpeed = map(ReadRightVibeSpeed,100,0,0,1024);
  analogWrite(RIGHT_PIN, RightVibeSpeed);

  //verbos
  Serial.print(RIGHT_HEADER);
  Serial.print(":");
  Serial.println(RightVibeSpeed);
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

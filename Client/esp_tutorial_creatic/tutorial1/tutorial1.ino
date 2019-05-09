
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>
#include <OSCBundle.h>
#include <OSCData.h>


//Wifi connection Adress
const char *ssid = "Patricio";
const char *password = "Patricio123";

WiFiUDP UDP;
const IPAddress outIp(192,168,0,1);          // remote IP (not needed for receive)
const unsigned int outPort = 12000;          // remote port (not needed for receive)
const unsigned int localPort = 10000;   
// Use WiFiClient class to create TCP connections
  WiFiClient client;


void setup() {
  // put your setup code here, to run once:
  //Serial DEBUG
  Serial.begin(115200);

  //WIFI Init
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
  
  // This will send the request to the server
  client.println("hello from ESP8266");

  //read back one line from server
  Serial.println("receiving from remote server");
  String line = client.readStringUntil('\r');
  Serial.println(line);
  Serial.println("closing connection");
  client.stop();
  //Launche UDP Connection
  LaunchUDP(localPort);
  
}

void loop() {
  // put your main code here, to run repeatedly:


  //Get GSR Value
  int Value = analogRead(A0); 
  int Time = millis();
  
  //Print Values
  //Serial.println(Value);
  //Serial.println(Time);
  
  //Send OSC Message
  OSCBundle bndl;
  UDP.beginPacket(outIp, outPort);
  OSCMessage msg("/Value");
  msg.add(Value);
  msg.add(Time); 
  msg.send(UDP);
  UDP.endPacket();
  msg.empty();



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


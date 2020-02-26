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

const char* ssid     = STASSID;
const char* password = STAPSK;

const char* host = "10.42.0.1";
//const uint16_t port = 12000;
const IPAddress outIp(10,42,0,1);          // remote IP (not needed for receive)
const unsigned int outPort = 12000;          // remote port (not needed for receive)
const unsigned int localPort = 10000; 

void setup() {
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
  //Serial.print("connecting to ");
  //Serial.print(host);
  //Serial.print(':');
  //Serial.println(port);

//  // Use WiFiClient class to create TCP connections
//  WiFiClient client;
//  if (!client.connect(host, port)) {
//    Serial.println("connection failed");
//    delay(5000);
//    return;
//  }

//  // This will send a string to the server
//  Serial.println("sending data to server");
//  if (client.connected()) {
//    client.println("hello from ESP8266");
//  }
//
//  // wait for data to be available
//  unsigned long timeout = millis();
//  while (client.available() == 0) {
//    if (millis() - timeout > 5000) {
//      Serial.println(">>> Client Timeout !");
//      client.stop();
//      delay(60000);
//      return;
//    }
//  }

//  // Read all the lines of the reply from server and print them to Serial
//  Serial.println("receiving from remote server");
//  // not testing 'client.connected()' since we do not need to send data here
//  while (client.available()) {
//    char ch = static_cast<char>(client.read());
//    Serial.print(ch);
//  }

  // Close the connection
//  Serial.println();
//  Serial.println("closing connection");
//  client.stop();
//
//  delay(300000); // execute once every 5 minutes, don't flood remote service

  
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

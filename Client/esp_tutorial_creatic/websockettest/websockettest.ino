#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>

const char* ssid     = "Patricio";
const char* password = "Patricio123";
const int pinLed0 = 13; 

WebSocketsServer webSocket = WebSocketsServer(81);

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t lenght) {
    Serial.printf("[%u] get Message: %s\r\n", num, payload);
    switch(type) {
        case WStype_DISCONNECTED:      
            break;
        case WStype_CONNECTED: 
            {
              IPAddress ip = webSocket.remoteIP(num);
              Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\r\n", num, ip[0], ip[1], ip[2], ip[3], payload);    
            }
            break;
        
        case WStype_TEXT:
            {
              //Serial.printf("[%u] get Text: %s\r\n", num, payload);
              String _payload = String((char *) &payload[0]);
              //Serial.println(_payload);
              
              String idLed = (_payload.substring(0,4));
              String intensity = (_payload.substring(_payload.indexOf(":")+1,_payload.length()));
              int intLed = intensity.toInt();
              Serial.print("Intensity: "); Serial.print(intensity); Serial.print(" to int "); Serial.println(intLed);
              updateLed (idLed, intLed);
              
            }   
            break;     
             
        case WStype_BIN:
            {
              hexdump(payload, lenght);
            }
            // echo data back to browser
            webSocket.sendBIN(num, payload, lenght);
            break;
  
    }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(pinLed0, OUTPUT); 
  WiFi.begin(ssid, password);

  while(WiFi.status() != WL_CONNECTED) {
     Serial.print(".");
     delay(200);
  }
    
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  delay(500);  
   
  Serial.println("Start Websocket Server");
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

void loop() {
  webSocket.loop();
}

void updateLed(String idLed, int intLed){
  int valPWM = map(intLed, 0, 99, 0, 254);
  Serial.println(valPWM);
  analogWrite(pinLed0, valPWM);
}

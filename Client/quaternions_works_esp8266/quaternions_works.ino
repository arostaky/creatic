#include <Wire.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <ESP8266WebServer.h>

#include <SPI.h>
#include "FS.h"

ESP8266WebServer server(80);

//const char* ssid = "********";
//const char* password = "********";
String ssid     = "Livebox-6DCE";
String password = "D3EA677F5EF6A42D755E35256D";

WiFiUDP Udp;
unsigned int localUdpPort = 4210;  // local port to listen on
char incomingPacket[255];  // buffer for incoming packets
char  replyPacket[] = "Hi there! Got the message :-)";  // a reply string to send back
void handleRoot ();


// MPU6050 Slave Device Address
const uint8_t MPU6050SlaveAddress = 0x68;

// Select SDA and SCL pins for I2C communication
const uint8_t scl = D7;
const uint8_t sda = D8;

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
IPAddress IP ;
String sip ;
char buf [1024];
void setup() {
  Serial.begin(115200);
  Wire.begin();//sda, scl);
  MPU6050_Init();
  WiFi.mode(WIFI_AP); //Our ESP8266-12E is an AccessPoint
  WiFi.softAP("coucou"); // Provide the (SSID, password); .
  server.begin(); // Start the HTTP Server
  IP = WiFi.softAPIP(); // Obtain the IP of the Server
  sprintf(buf, "%d.%d.%d.%d", IP[0], IP[1], IP[2], IP[3]);
  sip = String(buf);
  buf[0] = 0;
  Serial.println(IP);
  Serial.println(sip);
  server.on("/", handlequat);
  server.on("/loadfile", handleloadfile);
  server.on("/data", handledata);
  server.on("/quat", handlequat);
  server.on("/spif",handlespiffs);
  server.onNotFound ( handleNotFound );
  Serial.printf("Connecting to %s ", ssid.c_str());

  /*
   * 
   WiFi.begin(ssid.c_str(), password.c_str());
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" connected");
*/
  IP = WiFi.softAPIP(); // Obtain the IP of the Server
  sprintf(buf, "%d.%d.%d.%d", IP[0], IP[1], IP[2], IP[3]);
  sip = String(buf);
  buf[0] = 0;
  Serial.println(IP);
  Serial.println(sip);

  Udp.begin(localUdpPort);
  Serial.printf("Now listening at IP %s, UDP port %d\n", /*WiFi.localIP().toString().c_str()*/sip.c_str(), localUdpPort);
  
  SPIFFS.begin();
  delay(100);
  File f = SPIFFS.open("/conf.txt", "r");
  if (!f) {
    Serial.println("conf.txt open failed");
  }
  else
  {
    Serial.println("====== Reading from SPIFFS file =======");

    for (int i = 1; 1 && f.available(); i++) {
      String s = f.readStringUntil('\n');
      Serial.print(i);
      Serial.print(":");
      Serial.println(s);
    }
    f.close();
  }
  Serial.println("Setup done");
}

void handledata()
{
  String d = data();
  server.send(200, "text/html", d);
  //Serial.println(d);
}

void handlequat()
{
  String d = dataQuat();
  server.send(200, "", d);
  //Serial.println(d);
}

void handleRoot()
{
  String d = data();
  server.send(200, "text/html", d);
}

void handlespiffs ()
{
  Serial.println("handle spiffs");
  delay(2);
  String out = "";
  for (int i = 0; i < server.args(); i++)
  {
    String finame = "/" + server.arg(i);
    File f = SPIFFS.open(finame, "r");
    if (!f) {
      Serial.println("file open failed");
      out += "\n\n>>>>>>>>>>>>>>>>>>>>>> file ";
      out += finame + " open failed\n" ;
    }
    else
    {
      Serial.println("====== Reading from SPIFFS file =======");

      for (int i = 1; 1 && f.available(); i++) {
        String s = f.readStringUntil('\n');
        out += s + "\n";
        Serial.print(i);
        Serial.print(":");
        Serial.println(s);
      }
      f.close();
    }
  }
  server.send(200, "text/html", out);
}
void handleloadfile()
{
  String out = "";
  
  out += data();
  /*out += "<ul>\n";
  for (int i = 0; i < server.args(); i++)
    out += String("<li>") + server.argName(i) + String(":") + server.arg(i) + String("</li>\n");
  out += String("</ul>\n<hr/>>>>>>>>>> URI:") + server.uri();*/
  server.send(200, "text/html", out);
}

void handleNotFound ()
{
  Serial.println("handle not found");
  delay(2);
  String out = "";
  String path = server.uri();
  String finame = path.substring(path.indexOf("/"));
  File f = SPIFFS.open(finame, "r");
  Serial.println(String("SPIFFS file:") + finame);
  if (!f) {
    Serial.println("file open failed");
    out += "\n\n>>>>>>>>>>>>>>>>>>>>>> file ";
    out += finame + " open failed\n" ;
  }
  else
  {
    Serial.println("====== Reading from SPIFFS file =======");

    for (int i = 1; 1 && f.available(); i++) {
      String s = f.readStringUntil('\n');
      s += "\r\n" ;
      out += s;
    }
    f.close();
  }
  server.send(200, "text/html", out);
}

void onDisconnected(const WiFiEventStationModeDisconnected& event)
{
//  connect();
}

void onDisconnectedAP(const WiFiEventSoftAPModeStationDisconnected& event)
{
  //connect();
}

void loop() {
/*  if (Udp.available())
  {
    int packetSize = Udp.parsePacket();
    if (packetSize)
    {
      // receive incoming UDP packets
      Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
      int len = Udp.read(incomingPacket, 255);
      if (len > 0)
      {
        incomingPacket[len] = 0;
      }
      Serial.printf("UDP packet contents: %s\n", incomingPacket);

      // send back a reply, to the IP address and port we got the packet from
      Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
      Udp.write(replyPacket);
      Udp.endPacket();
    }
  }*/
  //String out = data();
  //Serial.println(out);
  /*Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
  Udp.write(out.c_str());
  Udp.endPacket();*/
  server.handleClient ();
}

String data()
{
  double Ax, Ay, Az, T, Gx, Gy, Gz;
  Read_RawValue(MPU6050SlaveAddress, MPU6050_REGISTER_ACCEL_XOUT_H);
  String out = "";
  //divide each with their sensitivity scale factor
  Ax = (double)AccelX / AccelScaleFactor;
  Ay = (double)AccelY / AccelScaleFactor;
  Az = (double)AccelZ / AccelScaleFactor;
  T = (double)Temperature / 340 + 36.53; //temperature formula
  Gx = (double)GyroX / GyroScaleFactor;
  Gy = (double)GyroY / GyroScaleFactor;
  Gz = (double)GyroZ / GyroScaleFactor;
  out += "Ax: " ;
  out += String(Ax);
  out += "Ay: " ;
  out += String(Ay);
  out += "Az: " ;
  out += String(Az);
  out += " T: " ;
  out += String(T);
  out += " Gx: ";
  out += String(Gx);
  out += " Gy: ";
  out += String(Gy);
  out += " Gz: ";
  out += String(Gz);
  out += "\n";
  return out;
}
String dataQuat()
{
  String out = "";
  out += String(Read_RawQuatValue(MPU6050SlaveAddress, MPU6050_REGISTER_ACCEL_XOUT_H));
  out += "\n";
  return out;
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

// this is message id as in User Manual
const char GET_QUAT_DATA = 0x02;    

// response is 16 bytes (4 x sizeof(float))
const char QUAT_DATA_SIZE = 16;   

// packet structure for InvenSense teapot demo
uint8_t teapotPacket[14] = { '$', 0x02, 0,0, 0,0, 0,0, 0,0, 0x00, 0x00, '\r', '\n' };

char * Read_RawQuatValue(uint8_t deviceAddress, uint8_t regAddress) {
  Wire.beginTransmission(deviceAddress);
  Wire.write(GET_QUAT_DATA);
  Wire.endTransmission();
  Wire.requestFrom(deviceAddress, (uint8_t)QUAT_DATA_SIZE);
  /*AccelX = (((int16_t)Wire.read() << 8) | Wire.read());
  AccelY = (((int16_t)Wire.read() << 8) | Wire.read());
  AccelZ = (((int16_t)Wire.read() << 8) | Wire.read());
  Temperature = (((int16_t)Wire.read() << 8) | Wire.read());
  GyroX = (((int16_t)Wire.read() << 8) | Wire.read());
  GyroY = (((int16_t)Wire.read() << 8) | Wire.read());
  GyroZ = (((int16_t)Wire.read() << 8) | Wire.read());*/
  int i=0;
  static char response[16],*teap;
    while (Wire.available()) {
        response[i++] = Wire.read();
    }
    teap=(char*)&teapotPacket;
  teapotPacket[2] = response[0];
            teapotPacket[3] = response[1];
            teapotPacket[4] = response[4];
            teapotPacket[5] = response[5];
            teapotPacket[6] = response[8];
            teapotPacket[7] = response[9];
            teapotPacket[8] = response[12];
            teapotPacket[9] = response[13];
            teap[14]=0;
            teapotPacket[11]++; // packetCount, loops at 0xFF on purpose
  /*float* quaternion = (float*)response;
  String respQuat = "";
  for (int a=0;a<14;a++)
    respQuat += String(teapotPacket[a],BIN);
  respQuat +="\nQ: ";
  respQuat += quaternion[0];
  respQuat += ", ";
  respQuat += quaternion[1];
  respQuat += ", ";
  respQuat += quaternion[2];
  respQuat += ", ";
  respQuat += quaternion[3];*/
  return teap ; //respQuat ;
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

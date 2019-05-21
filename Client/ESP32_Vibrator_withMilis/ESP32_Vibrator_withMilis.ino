#include <Arduino.h>
#include <Ticker.h>

// attach a LED to GPIO 21
#define LED_PIN 15

Ticker tickerSetHigh;
Ticker tickerSetLow;

void setPin(int state) {
  digitalWrite(LED_PIN, state);
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  //digitalWrite(1, LOW);
  
  // every 25 ms, call setPin(0) 
  tickerSetLow.attach_ms(200, setPin, 1);
   // every 25 ms, call setPin(0) 
  tickerSetLow.attach_ms(100, setPin, 0);
   // every 25 ms, call setPin(0) 
  tickerSetLow.attach_ms(200, setPin, 1);
  // every 26 ms, call setPin(1)
  tickerSetHigh.attach_ms(300, setPin, 0);
}

void loop() {

}

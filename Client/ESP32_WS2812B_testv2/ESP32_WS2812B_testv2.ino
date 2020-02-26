#include <NeoPixelBus.h>

NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(1, 4);

int brightness=32;
RgbColor yellow(brightness, brightness/4*3, 0);
RgbColor blue(0, 0, brightness);

portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
unsigned long interruptTime=millis();
bool blinkNow=false;
unsigned int resetTimer=millis();
bool resetLed=false;


void IRAM_ATTR handleInterrupt() {
  portENTER_CRITICAL_ISR(&mux);
  if(millis()>interruptTime+200) {
    interruptTime=millis();
    blinkNow=true;
  }
  portEXIT_CRITICAL_ISR(&mux);
}

void setup() {
  pinMode(12, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(12), handleInterrupt, FALLING);

  strip.Begin();
  strip.Show();
}

void loop() {
  if(blinkNow) {
    portENTER_CRITICAL(&mux);
    blinkNow=false;
    portEXIT_CRITICAL(&mux);
    
    resetLed=true;
    resetTimer=millis();
    //strip.Begin();
    strip.SetPixelColor(0, yellow);
    strip.Show();
  }
  if(resetLed) {
    if(resetTimer+100<millis()) {
      //strip.Begin();
      strip.SetPixelColor(0, blue);
      strip.Show();
      resetLed=false;
    }
  }
}

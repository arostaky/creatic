#define FASTLED_ESP32_I2S
#include <FastLED.h>
// How many leds in your strip?
#define NUM_LEDS 2
#define DATA_PIN 2

// Define the array of leds
CRGB leds[NUM_LEDS];

void setup() {
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
}

void loop() {
  // Turn the LED on, then pause
  FastLED.show();
  leds[0] = CHSV(random8(255), 255, 255);
  leds[1] = CHSV(random8(255), 255, 255);
  FastLED.delay(33);
  leds[0] = CHSV(random8(255), 255, 255);
  leds[1] = CHSV(random8(255), 255, 255);
  FastLED.delay(33);
  leds[0] = CHSV(random8(255), 255, 255);
  leds[1] = CHSV(random8(255), 255, 255);
  FastLED.delay(33);
  leds[0] = CHSV(random8(255), 255, 255);
  leds[1] = CHSV(random8(255), 255, 255);
  FastLED.delay(33);
  leds[0] = CHSV(random8(255), 255, 255);
  leds[1] = CHSV(random8(255), 255, 255);
  FastLED.delay(33);
  leds[0] = CHSV(random8(255), 255, 255);
  leds[1] = CHSV(random8(255), 255, 255);
  FastLED.delay(33);
  leds[0] = CRGB::Blue;
  leds[1] = CRGB::Blue;
  FastLED.delay(1000);
}

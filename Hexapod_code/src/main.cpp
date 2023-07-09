#include <Arduino.h>
#include <header.h>

// #include <FastLED.h>

#ifdef WS2812B_LED
CRGB leds[NUM_LEDS];
#endif

int loopTime;

void setup()
{

  // pinMode(PA8, INPUT);
  // pinMode(PC13, OUTPUT);

  Serial.begin(9600);

#ifdef WS2812B_LED
  FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS); // GRB ordering is typical

  FastLED.setBrightness(3);
#endif

#ifdef SERVO
  Servo_init();
#endif

  delay(10);
}

void loop()
{
  // int long curTime = millis();

#ifdef WS2812B_LED
  for (size_t i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = CRGB::Red;
    FastLED.show();
    delay(100);
  }

  for (size_t i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = CRGB::Green;
    FastLED.show();
    delay(100);
  }

  for (size_t i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = CRGB::Blue;
    FastLED.show();
    delay(100);
  }

#endif

  Servo_update();
  // delay(500);

  // Serial.println("end");

  // loopTime = millis() - curTime;
}
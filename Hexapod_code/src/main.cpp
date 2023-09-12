#include <Arduino.h>
#include <header.h>

int loopTime;

Vector2 directionInput(100, 0);
int rotaionInput = 0;
int groundClearance = 35;
int stepRadius = 100;

void setup()
{

#ifdef WS2812B_LED // initialitze LEDs
  Led_init();

  // turn all led off
  for (size_t i = 0; i < NUM_LEDS + 1; i++)
  {
    Led_update(i, CRGB::Black);
  }
  FastLED.show();
#endif

#ifdef SERVO // initialite Servos
  Servo_init();
#endif

#ifdef WS2812B_LED // startup LED animation
  for (size_t i = 0; i < 25; i++)
  {
    Led_update(i, CRGB::Blue);
    FastLED.show();
    delay(25);
  }

  for (int i = 0; i < 100; i++)
  {
    for (int o = 0; o < 6; o++)
    {
      LED_leg_animation(o, 0, float(i + 1) / 100.0f);
    }
    FastLED.show();

    delay(10);
  }

#endif

  standUp();
}

void loop()
{
  int long curTime = millis();

  walkCycle();

  Output_update();

  // delay(10);

  loopTime = millis() - curTime;
}
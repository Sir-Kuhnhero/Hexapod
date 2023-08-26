#include <Arduino.h>
#include <header.h>

int loopTime;

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

  delay(10);

#ifdef WS2812B_LED // startup LED animation
  for (size_t i = 0; i < 25; i++)
  {
    Led_update(i, CRGB::Blue);
    FastLED.show();
    delay(25);
  }
#endif

#ifdef SERVO // move Legs to start and stand up
  // move up to start
  Vector3 startPosition(50.0f, 0.0f, 120.0f);

  for (int i = 0; i < 6; i++)
  {
    Leg_update(i, startPosition);
    delay(250);
  }

  // slowly move legs down
  Vector3 curPosition(startPosition.x, startPosition.y, startPosition.z);

  for (curPosition.z = startPosition.z; curPosition.z > 0; curPosition.z--)
  {
    for (int i = 0; i < 6; i++)
    {
      Leg_update(i, curPosition);
    }
    delay(5);
  }

  // reposition legs to zero
  startPosition = curPosition;
  Vector3 liftPositio(curPosition.x / 2, curPosition.y, 50);
  Vector3 downPosition;

  int steps = 50;    // steps for interpolation
  int timeDelta = 2; // tiem between steps

#pragma region repositionLeg_0_2_4
  for (int i = 0; i < steps; i++)
  {
    curPosition = interpolate(startPosition, liftPositio, float(i) / float(steps));

    Leg_update(0, curPosition);
    Leg_update(2, curPosition);
    Leg_update(4, curPosition);

    delay(timeDelta);
  }
  Debug_Led(0b00000010);

  // move last 3 legs down
  for (int i = 0; i < steps; i++)
  {
    curPosition = interpolate(liftPositio, downPosition, float(i) / float(steps));

    Leg_update(0, curPosition);
    Leg_update(2, curPosition);
    Leg_update(4, curPosition);

    delay(timeDelta);
  }
  Debug_Led(0b00000011);

#pragma endregion

#pragma region repositionLeg_1_3_5
  for (int i = 0; i < steps; i++)
  {
    curPosition = interpolate(startPosition, liftPositio, float(i) / float(steps));

    Leg_update(1, curPosition);
    Leg_update(3, curPosition);
    Leg_update(5, curPosition);

    delay(timeDelta);
  }
  Debug_Led(0b00000100);

  // move last 3 legs down
  for (int i = 0; i < steps; i++)
  {
    curPosition = interpolate(liftPositio, downPosition, float(i) / float(steps));

    Leg_update(1, curPosition);
    Leg_update(3, curPosition);
    Leg_update(5, curPosition);

    delay(timeDelta);
  }
  Debug_Led(0b00000101);

#pragma endregion

#endif
}

void loop()
{
  // int long curTime = millis();

  // loopTime = millis() - curTime;
}
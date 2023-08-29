#include <Arduino.h>
#include <header.h>

int loopTime;

void testLegMovement();

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
  Vector3 positon(0, 0, 40);
  Leg_update(0, positon);
  Leg_update(1, positon);
  Leg_update(2, positon);
  Leg_update(3, positon);
  Leg_update(4, positon);
  Leg_update(5, positon);
  delay(1000);
  /*
    Leg_update(0, positon);
    delay(1000);

    positon.x = 75,
    positon.y = 0,
    Leg_update(0, positon);
    delay(1000);
    positon.x = -75,
    // positon.y = 75,
        Leg_update(0, positon);
    delay(1000);

    Debug_Led_8bit(5);
    delay(1000);
  */

  // begin tests
  Vector2 point(0, 0);
  Vector2 direction(10, 0);

  Vector2 result(0, 0);

  result = projectPointToCircle(75, point, direction);

  Debug_Led_16bit(int(result.x));
  Debug_Led_8bit(int(result.y));

  positon.x = result.x,
  positon.y = result.y,

  Leg_update(0, positon);
  Leg_update(1, positon);
  Leg_update(2, positon);
  Leg_update(3, positon);
  Leg_update(4, positon);
  Leg_update(5, positon);

  delay(50000000);

#ifdef SERVO // move Legs to start and stand up
  // move up to start
  Vector3 startPosition(0.0f, 0.0f, 120.0f);

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
    curPosition = Vector3::Lerp(startPosition, liftPositio, float(i) / float(steps));

    Leg_update(0, curPosition);
    Leg_update(2, curPosition);
    Leg_update(4, curPosition);

    delay(timeDelta);
  }

  // move last 3 legs down
  for (int i = 0; i < steps; i++)
  {
    curPosition = Vector3::Lerp(liftPositio, downPosition, float(i) / float(steps));

    Leg_update(0, curPosition);
    Leg_update(2, curPosition);
    Leg_update(4, curPosition);

    delay(timeDelta);
  }

#pragma endregion

#pragma region repositionLeg_1_3_5
  for (int i = 0; i < steps; i++)
  {
    curPosition = Vector3::Lerp(startPosition, liftPositio, float(i) / float(steps));

    Leg_update(1, curPosition);
    Leg_update(3, curPosition);
    Leg_update(5, curPosition);

    delay(timeDelta);
  }

  // move last 3 legs down
  for (int i = 0; i < steps; i++)
  {
    curPosition = Vector3::Lerp(liftPositio, downPosition, float(i) / float(steps));

    Leg_update(1, curPosition);
    Leg_update(3, curPosition);
    Leg_update(5, curPosition);

    delay(timeDelta);
  }

  delay(5000);

#pragma endregion

#endif
}

void loop()
{
  // int long curTime = millis();

  int minValue = -75;
  int maxValue = 75;

  for (int i = 0; i < minValue; i++)
  {
    Vector3 position(i, 0, 0);
    Leg_update(0, position);
    Leg_update(1, position);
    Leg_update(2, position);
    Leg_update(3, position);
    Leg_update(4, position);
    Leg_update(5, position);

    delay(10);
  }

  for (int i = minValue; i < maxValue; i++)
  {
    Vector3 position(i, 0, 0);
    Leg_update(0, position);
    Leg_update(1, position);
    Leg_update(2, position);
    Leg_update(3, position);
    Leg_update(4, position);
    Leg_update(5, position);

    delay(10);
  }

  for (int i = maxValue; i > minValue; i--)
  {
    Vector3 position(i, 0, 0);
    Leg_update(0, position);
    Leg_update(1, position);
    Leg_update(2, position);
    Leg_update(3, position);
    Leg_update(4, position);
    Leg_update(5, position);

    delay(10);
  }

  // loopTime = millis() - curTime;
}

void testLegMovement()
{
  int minValue = 0;
  int maxValue = 50;

  for (int i = 0; i < minValue; i++)
  {
    Vector3 position(0, i, 0);
    Leg_update(0, position);
    Leg_update(1, position);
    Leg_update(2, position);
    Leg_update(3, position);
    Leg_update(4, position);
    Leg_update(5, position);

    delay(10);
  }

  for (int i = minValue; i < maxValue; i++)
  {
    Vector3 position(0, i, 0);
    Leg_update(0, position);
    Leg_update(1, position);
    Leg_update(2, position);
    Leg_update(3, position);
    Leg_update(4, position);
    Leg_update(5, position);

    delay(10);
  }

  for (int i = maxValue; i > minValue; i--)
  {
    Vector3 position(0, i, 0);
    Leg_update(0, position);
    Leg_update(1, position);
    Leg_update(2, position);
    Leg_update(3, position);
    Leg_update(4, position);
    Leg_update(5, position);

    delay(10);
  }
}
#include <Arduino.h>
#include <header.h>

int loopTime;

void setup()
{

  Serial.begin(115200);
  pinMode(PC13, OUTPUT);

#ifdef DEBUG_ARDUINO

  pinMode(DEBUG_START, OUTPUT);
  pinMode(DEBUG_CLOCK, OUTPUT);
  pinMode(DEBUG_DATA, OUTPUT);
  pinMode(DEBUG_LINE_RETURN, OUTPUT);

#endif

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

  while (true)
  {
    Serial.println("hi");

    digitalWrite(PC13, HIGH);
    delay(250);
    digitalWrite(PC13, LOW);
    delay(250);
  }

  LegStartup();
}

void loop()
{
  int long curTime = millis();

  Vector2 direction(50, 0); // in what direction and how fast is the hexapod walking
  int height = 30;          // how heigh of the groung is the hexapod walking
  int stepRadius = 100;     // how long are the steps taken
  int rotation = 0;

  // walkCycle(direction, height, rotation, stepRadius);

  // Output_update();

  // delay(10);

  Debug_TransferData(float(loopTime));

  loopTime = millis() - curTime;
}
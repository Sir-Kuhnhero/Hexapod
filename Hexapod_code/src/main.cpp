#include <Arduino.h>
#include <header.h>

int loopTime;

Vector2 directionInput(0, 0);
float rotationInput = 20;
float groundClearance = 35;
float stepRadius = 100;
float maxSpeed = 90;

State HexapodState = State::SITTING;

#ifdef WS2812B_LED
int ledUpdateTimer = 0;
int brightness = 5;
int colorR = 0;
int colorG = 0;
int colorB = 255;
bool ledUpdate = true;
#endif

void setup()
{
#ifdef BLUETOOTH
  Bluetooth_init();

  // don't continue until a connection has been established and a array of data has been recived
  while (Data[DATA_LENGTH - 1] == 0) // no data send can be 0
  {
    Bluetooth_read();
    delay(1);
  }

  Bluetooth_map();
#endif

#ifdef WS2812B_LED // initialitze LEDs
  Led_init();
#endif

#ifdef SERVO // initialite Servos
  Servo_init();
#endif

#ifdef WS2812B_LED // startup LED animation
  for (size_t i = 0; i < 25; i++)
  {
    Led_update(i);
    FastLED.show();
    delay(25);
  }

#ifdef SERVO
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

#endif

#ifdef SERVO
#ifdef BLUETOOTH
  Bluetooth_clear();
  Data[0] = 50;

  while (Data[0] == 50) // is 50 when it should sit down and 100 when it should stand up
  {
    Bluetooth_read();
    delay(10);
  }

  standUp();
#endif
#endif

#ifdef SERVO_CALIBRATION
  HexapodState = State::STANDING;
#endif
}

void loop()
{
  int long curTime = millis();

#ifdef BLUETOOTH
  Bluetooth_read();
  Bluetooth_map();
#endif

#ifdef SERVO

  // stand up and sit down as input suggests
  if (Data[0] == 50 && (HexapodState == State::STANDING || HexapodState == State::WALKING))
  {
    sitDown();
    curTime = millis(); // the sitDown function takes time so loopTime gets messed up
  }
  else if (Data[0] == 100 && HexapodState == State::SITTING)
  {
    standUp();
    curTime = millis(); // the sitDown function takes time so loopTime gets messed up
  }

  if (HexapodState == State::STANDING || HexapodState == State::WALKING)
  {
    walkCycle();
    Output_update();
  }
#endif

#ifdef WS2812B_LED
  // ping pong led blue color
  /*
    if (colorBDirection == 0)
    {
      colorB--;
    }
    else if (colorBDirection == 1)
    {
      colorB++;
    }

    if (colorB == 0)
    {
      colorBDirection = 1;
    }
    else if (colorB == 255)
    {
      colorBDirection = 0;
    }
  */

  if (ledUpdate || (ledUpdateTimer > updateInterval && continuesUpdate))
  {

    for (size_t i = 0; i < NUM_LEDS; i++)
    {
      Led_update(i);
    }

    FastLED.setBrightness(brightness);
    FastLED.show();

    ledUpdate = false;
    ledUpdateTimer = 0;

#ifdef BLUETOOTH
    // Bluetooth_clear(); // updating every led takes time
#endif
    curTime = millis();
  }

  ledUpdateTimer += loopTime;
#endif

  loopTime = millis() - curTime;
}
#include <Arduino.h>
#include <header.h>

int loopTime;

Vector2 directionInput(0, 0);
float rotationInput = 20;
float groundClearance = 35;
float stepRadius = 100;

size_t couldread = 0;

#define txPin PA9
#define rxPin PA10 // only pins 10 & 11 work

SoftwareSerial bluetoothSerial(rxPin, txPin);

int ledPin = PC13; // LED connected to digital pin 13

void setup()
{
  bluetoothSerial.begin(9600); // Default communication rate of the Bluetooth module
  pinMode(PC13, OUTPUT);
  digitalWrite(PC13, HIGH);

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

#ifdef SERVO
  standUp();
#endif
}

void loop()
{
  int long curTime = millis();

#ifdef SERVO
  walkCycle();
  Output_update();
#endif

  // Check if data is available from the Bluetooth module
  if (bluetoothSerial.available() > 0)
  {

    char state = bluetoothSerial.read(); // Reads the data from the bluetoothSerial port

    bluetoothSerial.write(state);

    // Serial.print("received: ");
    // Serial.println(state);

    couldread++;
    digitalWrite(PC13, LOW);
  }

  // if (Serial.available())
  //{
  //   char state = Serial.read();
  //   bluetoothSerial.write(state);
  //   Serial.print(state);
  // }

  // Serial.print(float(loopTime));

  loopTime = millis() - curTime;
}
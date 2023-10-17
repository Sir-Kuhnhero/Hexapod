# Overview #

Seems like you want to build a Hexapod. Don't know why but since you're here, might as well tell you about the most important features about my design:

1. Cheap: 120 € (see Parts (toBuy) file)
2. RGB: 271 individual RGB LEDs
3. Cable management: Cables are routed inside the leg. no cable loop that tangle
4. ORP compatible: compatible with the Open Robotic Platform modules
5. Bluetooth control: Remote control via phone app

If this hasn't convinced you might I suggest watching my videos on the design and build.

Leg: https://youtu.be/Ta3ILBvA57U

Body & first Steps: https://youtu.be/FBk1Pa01zQ4

# Build #

Since you are reading this I assume you are at least considering my design. I have some instructions on Instucatbles if you want to take a look at that: https://www.instructables.com/3D-Printed-Hexapod/

You can also find all the most important files on Printables: https://www.printables.com/model/606030-3d-printed-hexapod

All the 3mf files are oriented correctly with the needed support material. My parts are design in a way that uses minimal support material. See “Parts (toPrint)” Excel sheet.



If you don't know what and where to buy take a look at the "Parts (toBuy)" Excel sheet. Those are the parts I bought and where. You can also update the prices and it will automatically calculate the final price. Everything I listed is without shipping since that varies drastically.

If I didn't buy it or the link doesn't work anymore I linked to where I would buy.

# Wiring #

![alt text](https://github.com/Sir-Kuhnhero/Hexapod/blob/main/Wiring%20Diagram%204K.png?raw=true)

Connect everything like in the wiring diagram above. Be sure to use shielded wire for the connection between the STM32 and the servo drivers. I had some problems with interference without it.

One thing missing here is the XT60 connection between the battery and the rest of the circuit. You need a way to shut the hexapod down.

The LEDs are all connected in series in this order: Eye Centre, Eye middle ring, Eye outer ring, bottom ring, leg[ (front right) root -> tip (proceeding clockwise)

For the servos you can connect them to wherever is easiest. However you will need to set the channel for each servo in the "output.cpp" file correctly.

On that node ensure that you bridge the first solder pad on one of the servo drivers. This will ensure that it uses a different I2C address. This second driver will continue the channel numbering of the first (16-31).

# Programming #

The code has been written in Visual Studio Code with the platform IO extension.

 

To Upload it make sure you have both installed and add the folder bellow as a project.

 

From here upload the code. Be sure to use an ST-Link for this. You can use a simple FTDI Programmer or something similar but you will have to unplug the Bluetooth module to connect it and change the "upload_protocol" in the platformio.ini file from "stlink" to what your programmer requires. Check here for more info.

```
[env:genericSTM32F103C8]
platform = ststm32
board = genericSTM32F103C8
framework = arduino
debug_tool = stlink
upload_protocol = stlink
lib_deps = 
  adafruit/Adafruit PWM Servo Driver Library@^2.4.1
  fastled/FastLED@^3.6.0
 ```

If you want to deactivate some functions, for example the LEDs, for some reason you can do that in the "header.h" file by commenting out what you don’t want. This can be great for testing new features.

```
#define WS2812B_LED
 
#define SERVO
 
#define DEBUG
// #define DEBUG_SERIAL
#define DEBUG_LED
 
#define BLUETOOTH
```

# Issues #

To be clear, my design is NOT perfect. Here are all the problems I have:

1. highly integrated leg design: routing the cables inside the leg looks awesome but makes assembly more difficult. If you were to not use LEDs this problem would largely be negated.
2. Current spikes: the hexapod is able to handle some load but especially at times of a power spike the Buck converter powering the servos can be insufficient. So you might want to use two or buy a more powerful one.
3. I2C buss instability: I had problems with the I2C buss between the STM32 and servo drivers breaking down causing the Hexapod to freeze. I was able to fix that by using shielded cables, but it required some trial and error.
4. incomplete Code: Not all features have been added in code. For example no LED animations beside the startup animation.


If you have any questions or suggestions consider writing me a comment.

Happy Making :)

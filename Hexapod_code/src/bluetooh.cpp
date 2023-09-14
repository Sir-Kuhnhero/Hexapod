#include <Arduino.h>
#include <header.h>

#ifdef BLUETOOTH

SoftwareSerial bluetoothSerial(rxBluetooth, txBluetooth);

size_t indexOfCurDataByte = 0;
bool readData = false;

int Data[8];

void Bluetooth_init()
{
    bluetoothSerial.begin(9600); // Default communication rate of the Bluetooth module
}

void Bluetooth_read()
{
    // Check if data is available from the Bluetooth module
    if (bluetoothSerial.available())
    {
        if (!readData)
        {
            if (bluetoothSerial.read() == 0) // 0 defines te begining of a new array of data being send. Without it we would get out of sync
            {
                readData = true;
                indexOfCurDataByte = 0;
            }
        }
        else // we are in sync so we read the next byte
        {
            Data[indexOfCurDataByte] = bluetoothSerial.read();

            indexOfCurDataByte++;

            if (indexOfCurDataByte >= 8) // we have reached the end of the expected receive data
            {
                readData = false;
                indexOfCurDataByte = 0;
            }
        }
    }
}

// maps receive data to directionInput etc.
void Bluetooth_map()
{
    // transfer received data to input Values for walking gait
    int stickZero = 100;
    int minValue = 20;

    maxSpeed = Data[5];

    rotationInput = float(Data[1] - stickZero) / float(stickZero - minValue) * maxSpeed * -1;

    directionInput.x = float(Data[4] - stickZero) / float(stickZero - minValue) * maxSpeed;
    directionInput.y = float(Data[3] - stickZero) / float(stickZero - minValue) * maxSpeed;

    directionInput = directionInput.inverse();

    groundClearance = Data[6];
    stepRadius = Data[7];
}

#endif
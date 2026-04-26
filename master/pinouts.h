#include "led.cpp"
#include "toggle.cpp"
#include "sensor.cpp"

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define OPERATIONAL_BTN false
#define MAINTENANCE_BTN true

// leds
const int estopButton = 7;
const int estopLed = 12;

const int dispatchButton = 9;
const int dispatchLed = 10;

const int eResetButton = 8; 
const int eResetLed = 11;

// toggles
const int opTop = 3;
const int opBottom = 4;

const int maintTop = 5;
const int maintBottom = 6;

// sensors
const int front = A0;
const int back = A0;
const int lift = A0;

// Button objects:
LED estop(estopButton, estopLed);
LED dispatch(dispatchButton, dispatchLed);
LED eReset(eResetButton, eResetLed);

Toggle operational(opTop, opBottom, OPERATIONAL_BTN);
Toggle maintenance(maintTop, maintBottom, MAINTENANCE_BTN);

// Sensor objects:
Sensor frontSensor(front);
Sensor backSensor(back);
Sensor liftSensor(lift);

// LCD display
LiquidCrystal_I2C lcd(0x27,16,2);

// Dummies for now
class Motor{
    public:
    void forward(){} // closed by default
    void reverse(){} // closed by default
    void stop(){} // open by default, unless no power
};

class Brake{
    public:
    void open(){}
    void close(){}
};

Motor motors;
Brake brakes;
#include "led.cpp"
#include "toggle.cpp"
#include "sensor.cpp"

#define OPERATIONAL_BTN false
#define MAINTENANCE_BTN true

// on/off
const int onOff = 13;

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
const int back = A1;
const int lift = A2;

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
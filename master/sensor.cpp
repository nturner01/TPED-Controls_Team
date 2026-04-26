#include <Arduino.h>

class Sensor{
private: 
int sensorPin;

public:
void setup(){
    pinMode(sensorPin, INPUT_PULLUP);
}
bool read(){
    return digitalRead(sensorPin);
}

Sensor(int sensorPin) : sensorPin(sensorPin) {}
};
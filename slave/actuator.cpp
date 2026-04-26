#include <Arduino.h>
#pragma once
class Actuator{
private:
int pin;

public:
void setup(){
    pinMode(pin, OUTPUT);
}
void open(){
    digitalWrite(pin, HIGH);
}
void close(){
    digitalWrite(pin, LOW);
}
Actuator(int pin) : pin(pin) {}
};


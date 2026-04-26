#include <Arduino.h>
#include "actuator.cpp"
#pragma once
class Motor{
private:
int enaPin;
int in1;
int in2;

Actuator a1;
Actuator a2;

public:
void setup(){
    pinMode(enaPin, OUTPUT);
    pinMode(in1, OUTPUT);
    pinMode(in2, OUTPUT);
}

void stop() {
    analogWrite(enaPin, 0);
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
}
void forward(int speed = 150){
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    analogWrite(enaPin, speed);
}
void reverse(int speed = 150){
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    analogWrite(enaPin, speed);
}

void close(){
    a1.close();
    a2.close();
}
void open(){
    a1.open();
    a2.open();
}
Motor(int enaPin, int in1, int in2, int a1Pin, int a2Pin) :
    enaPin(enaPin), in1(in1), in2(in2),
    a1(a1Pin), a2(a2Pin) {}
};
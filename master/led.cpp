#include <Arduino.h>

class LED{
private:
int buttonPin;
int ledPin;
unsigned long prevTime;

public:
void blink(int inveral){
    unsigned long currTime = millis();
    if(currTime - prevTime >= interval){
        prevTime = currTime;
        digitalWrite(ledPin, !digitalRead(ledPin))
    }
}
LED(int buttonPin , int ledPin) : buttonPin(buttonPin), ledPin(ledPin), prevTime(0) {}
};

LED estop();

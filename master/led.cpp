#include <Arduino.h>

class LED {
private:
    int ledPin;
    int buttonPin;

public:
    LED(int buttonPin, int ledPin) : buttonPin(buttonPin), ledPin(ledPin), prevTime(0) {}

    void setup() {
        pinMode(buttonPin, INPUT_PULLUP); 
        pinMode(ledPin, OUTPUT);
    }

    void on(){
        digitalWrite(ledPin, HIGH);
    }

    void off(){
        digitalWrite(ledPin, LOW);
    }

    bool read(){
        return !digitalRead(buttonPin);
    }

    void blink(unsigned long interval = 750) {
        bool on = (millis() / interval) % 2 == 0;
        digitalWrite(ledPin, on);
    }
};

#include <Arduino.h>

class LED {
private:
    int ledPin;
    unsigned long prevTime;
    bool ledState;

public:
    LED(int ledPin) : ledPin(ledPin), prevTime(0), ledState(false) {}

    void begin() {
        pinMode(ledPin, OUTPUT);
        digitalWrite(ledPin, LOW);
    }

    // buttonState: LOW -> pressed, HIGH -> not pressed
    void update(int buttonState, unsigned long interval = 500) {
        // if the button is pressed, turn off LED
        if (buttonState == LOW) {
            ledState = false;
            digitalWrite(ledPin, LOW);
            return;
        }

        // if the button isn't pressed, blink
        unsigned long currTime = millis();
        if (currTime - prevTime >= interval) {
            prevTime = currTime;
            ledState = !ledState;
            digitalWrite(ledPin, ledState ? HIGH : LOW);
        }
    }
};
// 13, 2 ???

LED estop(13);

const int buttonPin = 2;

void setup() {
    pinMode(buttonPin, INPUT_PULLUP); 
    estop.begin();
}

void loop() {
    int buttonState = digitalRead(buttonPin); 
    estop.update(buttonState, 500);
}

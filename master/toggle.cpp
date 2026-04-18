# include <Arduino.h>
# include "enums.h"

class Toggle {
private:
    bool topPin;
    bool botPin;
    bool button; // 0/false for op mode button, 1/true for maint mode button

public:
    void setup(){
        pinMode(topPin, INPUT_PULLUP);
        pinMode(botPin, INPUT_PULLUP);
    }

    ToggleReading read() {
        bool topTemp = digitalRead(topPin);
        bool botTemp = digitalRead(botPin);
        if (topTemp && botTemp) {
            return !button ? AUTOMATIC_MODE : NEUTRAL_MODE;
        }
        else if (!topTemp) {
            return !button ? CONTINUOUS_MODE : REVERSE_MODE;
        }
        else if (!botTemp) {
            return !button ? MAINTENANCE_MODE : FORWARD_MODE;
        }
    }

Toggle(bool topPin, bool botPin, bool button):
    topPin(topPin), botPin(botPin), button(button) {}

};
# include <Arduino.h>
# include "pinouts.h"

class Toggle {
    
    private:
        bool topPin;
        bool botPin;
        int button; // 0 for op mode button, 1 for maint mode button

    public:

        ToggleState readOpMode() {
            bool topTemp = digitalRead(topPin);
            bool botTemp = digitalRead(botPin);
            if (topTemp && botTemp) {
                return button == 0 ? AUTOMATIC_MODE : NEUTRAL_MODE;
            }
            else if (!topTemp) {
                return button == 0 ? CONTINUOUS_MODE : REVERSE_MODE;
            }
            else if (!botTemp) {
                return button == 0 ? MAINTENANCE_MODE : FORWARD_MODE;
            }
        }





        Toggle(bool topPin, bool botPin, int button):
            topPin(topPin), botPin(botPin), button(button) {}

}
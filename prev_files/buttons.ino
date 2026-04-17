const int pinDispatchInput = 0;
const int pinDispatchLight = 0;

void setup(){
    pinMode(pinDispatchInput, INPUT_PULLUP);
    pinMode(pinDispatchLight, OUTPUT);
    Serial.begin(9600);
}

void loop(){
    // Only bother checking for inputs if the ride can actually be dispatched
    if (digitalRead(pinDispatchInput) == LOW /* && state == idleReady */) {
        //set state to dispatched
    }

    // Have the light visually indicate that the button only does anything if the ride is ready to dispatch
    if (/*state == idleReady*/) {digitalWrite(pinDispatchLight, HIGH)}
    else                        {digitalWrite(pinDispatchLight, LOW )}
}


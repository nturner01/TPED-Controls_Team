

const int pin1 = 2; // forward
const int pin2 = 3; // reverse


int lastForward = HIGH;
int lastReverse = HIGH;

void setup(){
    pinMode(pin1, INPUT_PULLUP);
    pinMode(pin2, INPUT_PULLUP);
    Serial.begin(9600);
}

void loop(){
    int forwardState = digitalRead(pin1);
    int reverseState = digitalRead(pin2);

    
    if (lastForward == HIGH && forwardState == LOW) {
        Serial.println("DISPATCH / FORWARD");
    }


    if (lastReverse == HIGH && reverseState == LOW) {
        Serial.println("REVERSE");
    }


    if (forwardState == HIGH && reverseState == HIGH &&
        (lastForward == LOW || lastReverse == LOW)) {
        Serial.println("NEUTRAL / IDLE");
    }

   
    if (forwardState == LOW && reverseState == LOW) {
        Serial.println("ERROR: invalid switch state");
    }

   
    lastForward = forwardState;
    lastReverse = reverseState;
}

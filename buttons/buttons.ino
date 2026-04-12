// Feel free to rename these to make your code more readable! Leave the values to 0, we'll assign these later
const int topLine = 10;
const int bottomLine = 7;

void setup(){
    /*
    If you have an LED button:
    pinMode(pin1, INPUT_PULLUP); --> sets up reading the pressing of the button
    pinMode(pin2, OUTPUT); --> sets up lighting up the LED

    If you have a toggle button:
    pinMode(pin1, INPUT_PULLUP); -->  sets up reading the top beam
    pinMode(pin2, INPUT_PULLUP); -->  sets up reading the botton beam
    */
	pinMode(topLine, INPUT_PULLUP);
	pinMode(bottomLine, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop(){
    /* Add whatever logic here you need to 
    1. Read the input of the button (maybe make it print something using Serial.println())
    2. Either read the second input (if you have a toggle) or manipulate the LED (if you have an LED button)

    Helpful functions:
    digitalRead(pin) --> returns the reading of the specified pin (LOW or HIGH).
        **NOTE: INPUT_PULLUP MEANS THAT A READING OF LOW MEANS THE BUTTON IS PRESSED, AND HIGH MEANS IT IS NOT!!!!!**
    digitalWrite(pin, VALUE) --> sets the pin to the VALUE specified, use either HIGH or LOW (use this for the LED)

    Note: If you have a pushbutton, you need to make sure your code recognizes when the button actually goes from not pressed to pressed, not just being held down!
    */
	// Get readings for both pins
	bool top = digitalRead(topLine);
	bool bottom = digitalRead(bottomLine);

	// Logic for reading of both HIGH
	if (top == HIGH && bottom == HIGH) {
		Serial.println("Mode: Auto");
	}

	// Logic for reading of pin1 LOW and pin2 HIGH
	else if (top == LOW) {
		Serial.println("Mode: Continuous Auto");
	}

	// Logic for reading of pin1 HIGH and pin2 LOW
	else if (bottom == LOW) {
		Serial.println("Mode: Maintenance");
	}

	delay(1000);
	
}


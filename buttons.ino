// Feel free to rename these to make your code more readable! Leave the values to 0, we'll assign these later
const int pin1 = 0;
const int pin2 = 0;

void setup(){
    /*
    If you have an LED button:
    pinMode(pin1, INPUT_PULLUP); --> reads the pressing of the button
    pinMode(pin2, OUTPUT); --> lights up the LED

    If you have a toggle button:
    pinMode(pin1, INPUT_PULLUP); --> reads the top beam
    pinMode(pin2, INPUT_PULLUP); --> reads the botton beam
    */
   Serial.begin(9600);
}

void loop(){
    /* Add whatever logic here you need to 
    1. Read the input of the button (maybe make it print something using Serial.println())
    2. Either read the second input (if you have a toggle) or manipulate the LED (if you have an LED button)

    
    */
}


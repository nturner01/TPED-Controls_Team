#include "enums.h"
#include "pinouts.h"

// for debugging/pre-slave readings
const char* states[] = {
    "IDLE",
    "READY",
    "REVERSE",
    "DISPATCHED",
    "DISPATCHED_2",
    "FORWARD",
    "IDLE_MAINT",
    "REVERSE_MAINT",
    "FORWARD_MAINT",
    "EMERGENCY"
};


FiniteState currState = IDLE;
FiniteState newState = IDLE;
FiniteState lastReading = IDLE;

// State management vars
int phase = 0;
bool ledHold = true;

//debouncing
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;

// dispatch delay
unsigned long dispatchedEnterTime = 0;
const unsigned long dispatchDelay = 2500;

void setup(){
    Serial.begin(9600);
    while (!Serial);  

    dispatch.setup();
    estop.setup();
    eReset.setup();
    
    operational.setup();
    maintenance.setup();

    backSensor.setup();
    frontSensor.setup();
    liftSensor.setup();
    
    delay(1000);
    Serial.println("Ride initalized");
    Serial.println("IDLE");
}

void loop(){
    switch(currState){
        case IDLE: {
            estop.blink();
            if(eReset.read() && ledHold){
                eReset.on();
            }
            else{
                ledHold = false;
                eReset.off();
            }
            
            if(estop.read()){
                ledHold = true;
                newState = EMERGENCY;
                break;
            }
            if(operational.read() == MAINTENANCE_MODE){
                newState = IDLE_MAINT;
                break;
            }
            if(frontSensor.read()){
                newState = READY;
                break;
            }
            
            motors.stop();
            brakes.close();
            break;
        }

        case READY: {
            dispatch.blink();
            estop.blink();

            if(estop.read()){
                newState = EMERGENCY;
                break;
            }
            if(operational.read() == MAINTENANCE_MODE){
                newState = IDLE_MAINT;
                break;
            }
            if(dispatch.read()){
                ledHold = true;
                newState = REVERSE;
                break;
            }
            if(operational.read() == CONTINUOUS_MODE){
                newState = REVERSE;
                break;
            }

            brakes.close();
            motors.stop();
            break;    
        }

        case REVERSE: {
            estop.blink();
            if(dispatch.read() && ledHold) dispatch.on();
            else{
                ledHold = false;
                dispatch.off();
            }

            if(estop.read()){
                newState = EMERGENCY;
                break;
            }
            if(liftSensor.read()){
                phase = 0;
                newState = DISPATCHED;
                break;
            }

            // NOTE: may need to change in the future for motor grouping using back sesnor
            motors.reverse();
            brakes.open();
            break;
        }

        case DISPATCHED:
        case DISPATCHED_2: {
            estop.blink();

            if(estop.read()){
                newState = EMERGENCY;
                break;
            }
            
            // Basic phase detection
            if (millis() - dispatchedEnterTime >= dispatchDelay) {
                newState = DISPATCHED_2;
                if (backSensor.read()) {
                    newState = FORWARD;
                    break;
                }
            }
            
            brakes.open();
            motors.stop();
            break;    
        }

        case FORWARD: {
            estop.blink();

            if(estop.read()){
                newState = EMERGENCY;
                break;
            }
            if(frontSensor.read()){
                newState = READY;
                break;
            }
            
            brakes.open();
            motors.forward();
            break;    
        }
        
        case IDLE_MAINT: {
            estop.blink();
            dispatch.off();

            if(estop.read()){
                newState = EMERGENCY;
                break;
            }
            if(operational.read() != MAINTENANCE_MODE){
                newState = IDLE;
                break;
            }
            if(maintenance.read() == REVERSE_MODE && !liftSensor.read()){
                newState = REVERSE_MAINT;
                break;
            }
            if(maintenance.read() == FORWARD_MODE && !frontSensor.read()){
                newState = FORWARD_MAINT;
                break;
            }
            
            brakes.close();
            motors.stop();
            break;
        }
 
        case FORWARD_MAINT: {
            estop.blink();
            dispatch.off();

            if(estop.read()){
                newState = EMERGENCY;
                break;
            }
            if(operational.read() != MAINTENANCE_MODE){
                newState = IDLE;
                break;
            }
            if(maintenance.read() == NEUTRAL_MODE || frontSensor.read()){
                newState = IDLE_MAINT;
                break;
            }
            if(maintenance.read() == REVERSE_MODE){
                newState = REVERSE_MAINT;
                break;
            }

            brakes.open();
            motors.forward();
            break;
        }

        case REVERSE_MAINT: {
            estop.blink();
            dispatch.off();

            if(estop.read()){
                newState = EMERGENCY;
                break;
            }
            if(operational.read() != MAINTENANCE_MODE){
                newState = IDLE;
                break;
            }
            if(maintenance.read() == NEUTRAL_MODE || liftSensor.read()){
                newState = IDLE_MAINT;
                break;
            }
            if(maintenance.read() == FORWARD_MODE){
                newState = FORWARD_MAINT;
                break;
            }

            brakes.open();
            motors.reverse();
            break;
        }

        case EMERGENCY: {
            dispatch.off();
            if(estop.read()){
                estop.on();
                eReset.off();
            }
            else{
                estop.off();
                eReset.blink();
            }

            if(!estop.read() && eReset.read()){
                // Note: Verify ride functionality before returning here. Currently just makes sure estop is up
                newState = IDLE;
                break;
            }

            brakes.close();
            motors.stop();
            break;
        }

        default:
            Serial.print("ERROR: ");
            Serial.println(currState);
            newState = IDLE;
            break;
    }

    if (newState != lastReading) {
        lastDebounceTime = millis();
    }

    if ((millis() - lastDebounceTime) >= debounceDelay) {
        if (currState != newState) {
            currState = newState;
            if (currState == DISPATCHED) {
                dispatchedEnterTime = millis();
            }
            // Send signal to slave here:
            Serial.println(states[currState]);
            /*
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Current state: ");
            lcd.setCursor(0, 1);
            lcd.print(states[currState]);
            */
        }
    }

    lastReading = newState;
}
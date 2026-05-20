#include "enums.h"
#include "pinouts.h"

// for debugging/pre-slave readings
const char* states[] = {
    "OFF",
    "Idle on track",
    "Read for dispatch",
    "Reversing up lift hill",
    "Phase 1 of dispatch sequence",
    "Phase 2 of dispatch sequence",
    "Phase 3 of dispatch sequence",
    "Return back to station",
    "Idle maintenance",
    "Reverse maintenance",
    "Forward maintenance",
    "E-Stop called"
};


FiniteState currState = IDLE;
FiniteState newState = IDLE;
FiniteState lastReading = IDLE;

// State management vars
bool ledHold = true;

//debouncing
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;

// cont auto delay
unsigned long contAutoEnterTime = 0;
const unsigned long contAutoDelay = 2500;

// dispatch delay
unsigned long dispatchedEnterTime = 0;
const unsigned long dispatchDelay = 2500;


void setup(){
    Serial.begin(9600);
    Serial1.begin(9600);

    dispatch.setup();
    estop.setup();
    eReset.setup();
    
    operational.setup();
    maintenance.setup();

    backSensor.setup();
    frontSensor.setup();
    liftSensor.setup();
    
    pinMode(onOff, INPUT_PULLUP);
    while(digitalRead(onOff) || !Serial || !Serial1);

    Serial.println("Ride initalized\nCurrent State: Idle on track");
}

void loop(){
    switch(currState){
    case OFF: {
        estop.off();
        eReset.off();
        dispatch.off();

        if(!digitalRead(onOff)){
            newState = IDLE;
            break;
        }
        break;
    }
    case IDLE: {
        estop.blink();
        if(eReset.read() && ledHold){
            eReset.on();
        }
        else{
            ledHold = false;
            eReset.off();
        }
        
        if(digitalRead(onOff)){
            newState = OFF;
            break;
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
        
        break;
    }

    case READY: {
        if(operational.read() == AUTOMATIC_MODE) dispatch.blink();
        else dispatch.off();

        estop.blink();

        if(digitalRead(onOff)){
            newState = OFF;
            break;
        }
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
        if(operational.read() == CONTINUOUS_MODE && millis() - contAutoEnterTime >= contAutoDelay){
            newState = REVERSE;
            break;
        }

        break;    
    }

    case REVERSE: {
        estop.blink();
        if(dispatch.read() && ledHold) dispatch.on();
        else{
            ledHold = false;
            dispatch.off();
        }

        if(digitalRead(onOff)){
            newState = OFF;
            break;
        }
        if(estop.read()){
            newState = EMERGENCY;
            break;
        }
        if(liftSensor.read()){
            newState = DISPATCHED;
            break;
        }

        break;
    }

    case DISPATCHED:
    case DISPATCHED_2: {
        estop.blink();

        if(digitalRead(onOff)){
            newState = OFF;
            break;
        }
        if(estop.read()){
            newState = EMERGENCY;
            break;
        }
        // Basic phase detection
        if (millis() - dispatchedEnterTime >= dispatchDelay) {
            newState = DISPATCHED_2;
            if (frontSensor.read()) {
                newState = DISPATCHED_3;
                break;
            }
        }
        
        break;    
    }

    case DISPATCHED_3: {
        estop.blink();

        if(digitalRead(onOff)){
            newState = OFF;
            break;
        }
        if(estop.read()){
            newState = EMERGENCY;
            break;
        }
        if(backSensor.read()){
            newState = FORWARD;
            break;
        }

        break;
    }

    case FORWARD: {
        estop.blink();

        if(digitalRead(onOff)){
            newState = OFF;
            break;
        }
        if(estop.read()){
            newState = EMERGENCY;
            break;
        }
        if(frontSensor.read()){
            newState = READY;
            break;
        }
        
        break;    
    }
    
    case IDLE_MAINT: {
        estop.blink();
        dispatch.off();

        if(digitalRead(onOff)){
            newState = OFF;
            break;
        }
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
        
        break;
    }

    case FORWARD_MAINT: {
        estop.blink();
        dispatch.off();

        if(digitalRead(onOff)){
            newState = OFF;
            break;
        }
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

        break;
    }

    case REVERSE_MAINT: {
        estop.blink();
        dispatch.off();
        
        if(digitalRead(onOff)){
            newState = OFF;
            break;
        }
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

        break;
    }

    case EMERGENCY: {
        dispatch.off();
        if(estop.read()){
            estop.on();
        }
        else{
            estop.off();
        }

        if(digitalRead(onOff)){
            newState = OFF;
            break;
        }
        // To clear, estop must be up and operational mode must be automatic 
        if(clearEmergency()){
            eReset.blink();
            if(eReset.read()){
                newState = IDLE;
                break;
            }     
        }
        else{
            eReset.off();
        }

        break;
    }

    default:
        Serial.print("State error");
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
            if (currState == READY) {
                contAutoEnterTime = millis();
            }
            // Print and send signal to slave here:
            Serial.print("Current State: ");
            Serial.println(states[currState]);

            Serial1.write((uint8_t)currState);
        }
    }
    lastReading = newState;
    delay(50);
}

bool clearEmergency(){
    return !estop.read() && operational.read() == AUTOMATIC_MODE;
}

// todo: add OFF state, adjust brakes class, and send signals to slave
#include "enums.h"
#include "pinouts.h"
#include "dummy.h"

CurrentState currState = IDLE;
CurrentState newState;
int phase = 0;

void setup(){
    Serial.begin(9600);
    delay(100);
    // set up classes here
    Serial.println("Ride initalized");
    Serial.println("Current state = IDLE");
}
void loop(){
    switch(currState){
        case IDLE: {
            if(readEStopButton()){
                newState = EMERGENCY;
                break;
            }
            if(readOperationalMode() == MAINTENANCE_MODE){
                newState = IDLE_MAINT;
                break;
            }
            if(readFrontSensor()){
                newState = READY;
                break;
            }
            motors.stop();
            brakes.close();
            break;
        }

        case READY: {
            if(readEStopButton()){
                newState = EMERGENCY;
                break;
            }
            if(readOperationalMode() == MAINTENANCE_MODE){
                newState = IDLE_MAINT;
                break;
            }
            if(readDispatchButton()){
                newState = REVERSE;
                break;
            }
            if(readOperationalMode() == AUTOMATIC_MODE){
                newState = REVERSE;
                break;
            }

            brakes.close();
            motors.stop();
            break;    
        }

        case REVERSE: {
            if(readEStopButton()){
                newState = EMERGENCY;
                break;
            }
            if(readLiftSensor()){
                phase = 0;
                newState = DISPATCHED;
                break;
            }

            // NOTE: may need to change in the future for motor grouping using back sesnor
            motors.reverse();
            brakes.open();
            break;
        }

        case DISPATCHED: {
            if(readEStopButton()){
                newState = EMERGENCY;
                break;
            }
            
            // TODO: Phase detection
            
            brakes.open();
            motors.stop();
            break;    
        }

        case FORWARD: {
            if(readEStopButton()){
                newState = EMERGENCY;
                break;
            }
            if(readFrontSensor()){
                newState = READY;
                break;
            }
            
            brakes.open();
            motors.forward();
            break;    
        }
        
        case IDLE_MAINT: {
            if(readEStopButton()){
                newState = EMERGENCY;
                break;
            }
            if(readOperationalMode() != MAINTENANCE_MODE){
                newState = IDLE;
                break;
            }
            if(readMaintenanceMode() == REVERSE_MODE){
                newState = REVERSE_MAINT;
                break;
            }
            if(readMaintenanceMode() == FORWARD_MODE){
                newState = FORWARD_MAINT;
                break;
            }
        

            brakes.close();
            motors.stop();
            break;
        }
 
        case FORWARD_MAINT: {
            if(readEStopButton()){
                newState = EMERGENCY;
                break;
            }
            if(readOperationalMode() != MAINTENANCE_MODE){
                newState = IDLE;
                break;
            }
            if(readMaintenanceMode() == NEUTRAL_MODE || readFrontSensor()){
                newState = IDLE_MAINT;
                break;
            }
            if(readMaintenanceMode() == REVERSE_MODE){
                newState = REVERSE_MAINT;
                break;
            }

            brakes.open();
            motors.forward();
            break;
        }

        case REVERSE_MAINT: {
            if(readEStopButton()){
                newState = EMERGENCY;
                break;
            }
            if(readOperationalMode() != MAINTENANCE_MODE){
                newState = IDLE;
                break;
            }
            if(readMaintenanceMode() == NEUTRAL_MODE || readLiftSensor()){
                newState = IDLE_MAINT;
                break;
            }
            if(readMaintenanceMode() == FORWARD_MODE){
                newState = FORWARD_MAINT;
                break;
            }

            brakes.open();
            motors.reverse();
            break;
        }

        case EMERGENCY: {
            if(!readEStopButton() && readEStopReset()){
                // Note: Verify ride functionality before returning here. Currently just makes sure estop is up
                newState = IDLE;
            }
            brakes.close();
            motors.stop();
        }

        default:
            Serial.println("ERROR");
            newState = IDLE;
            break;
    }

    if(currState != newState){
        currState = newState;
        Serial.print("Current state = ");
        Serial.println(currState);
    }
}
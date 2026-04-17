#include <iostream>
#include "classes.h"
#include "inputs.h"

enum CurrentState{
    IDLE, // IDLE BLOCKED
    READY, // IDLE READY
    REVERSE, // REVERSE BLOCKED
    DISPATCHED, // DISPATCHED BLOCKED
    FORWARD, // FORWARD BLOCKED
    IDLE_MAINT, // IDLE MAINTENANCE
    REVERSE_MAINT, // REVERSE MAINTENANCE
    FORWARD_MAINT, // FORWARD MAINTENANCE
    EMERGENCY, // IDLE EMERGENCY
};


Motor motors; // reverse, forward, stop
Break breaks; // open, close

// See inputs.h for dummy functions

// Default when ride starts. Change this if you want to test your code!
enum CurrentState state = IDLE; 
int main(){
    switch(state){
        /*
        Example of IDLE for reference
        Each case should do the following:
            1. Read buttons with valid transitions using dummy functions from inputs.h
            2. Change state and break. Notice checking the ordering is deliberately from most to least precedence
            3. Act on current state.
        */
        case IDLE: 
        {
            // Part 1: Read buttons
            bool estop = readEStopButton();
            bool station = readStationSensor();
            OperationalMode reading = readOperationalMode();

            // Part 2: Check buttons and update states.
            // Look at FSM for this!
            if(estop == HIGH){
                state = EMERGENCY;
                break;
            }
            if(reading == MAINTENANCE_MODE){
                state = IDLE_MAINT;
                break;
            }
            if(station == HIGH){
                state = READY;
                break;
            }

            // Part 3: Act on current state if we're still here
            std::cout << "Current state = IDLE" << std::endl;
            breaks.close();
            motors.stop();
            break;
        }
        
        case REVERSE:
        {
            bool liftSensor = readLLiftSensor();
            bool estop = readEStop();
            if(liftSensor == HIGH){
                state = DISPATCHED;
                break;
            }
            if(estop == HIGH){
                state = EMERGENCY;
                break;
            }
        }
        // TODO: Implement your case statement here (don't delete the default statement or you'll get an error)
        // Change the "state" variable above to your state if you want to test your code!
        default:
            std::cout << "If this prints you did something wrong :P\n";
            break;
    }
}
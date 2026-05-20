#include "pinouts.h"
#include "enums.h"
#include <Wire.h>

FiniteState newState = OFF;
FiniteState oldState = OFF;

void setup(){
  brakes.setup();
  brakes.disable();

  motors.setup();
  motors.off();

  acts.setup();
  acts.disable();
  Serial1.begin(9600);
}

void loop() {
  if (Serial1.available()) {
      newState = (FiniteState)Serial1.read();

    }

    if(oldState != newState){
      switch(newState) {
        case OFF:
        case IDLE:
        case READY:
        case DISPATCHED_2:
        case EMERGENCY:
          motors.off();
          brakes.disable();
          acts.disable();
          break;

        case DISPATCHED:
        case DISPATCHED_3:
          motors.off();
          brakes.open();
          acts.open();
          break;

        case FORWARD:
          motors.forward();
          brakes.open();
          acts.close();
          break;

        case REVERSE:
          motors.reverse();
          brakes.open();
          acts.close();
          break;
        
        case IDLE_MAINT:
          motors.off();
          brakes.close();
          //acts.close();
          break;

        case FORWARD_MAINT:
          motors.forward();
          brakes.open();
          acts.close();
          break;

        case REVERSE_MAINT:
          motors.reverse();
          brakes.open();
          acts.close();
          break;
          
        default:
          motors.off();
          brakes.disable();
          acts.disable();
          break;
      }
    oldState = newState;
  }
  delay(10);
}
#include <Arduino.h>
#include "enums.h"
#pragma once

class Solenoid{
private:
  int ena;
  int oddIn;
  int evenIn;
  bool type;
  bool isOpen;

public:
  void setup(){
    pinMode(ena, OUTPUT);
    pinMode(oddIn, OUTPUT);
    pinMode(evenIn, OUTPUT);
  }

  void open(){
    if(!isOpen){
      isOpen = true;
      if(type){
      digitalWrite(oddIn, LOW);
      digitalWrite(evenIn, LOW);
      digitalWrite(ena, HIGH);
    } else {
      digitalWrite(oddIn, LOW);
      digitalWrite(evenIn, HIGH);
      digitalWrite(ena, HIGH);
    }
    }
  }

  void close(){
    if(isOpen){
      isOpen = false;
      if(type){
      digitalWrite(oddIn, LOW);
      digitalWrite(evenIn, HIGH);
      digitalWrite(ena, HIGH);
    } else {
      digitalWrite(oddIn, LOW);
      digitalWrite(evenIn, LOW);
      digitalWrite(ena, HIGH);
    }
    }
  }

  void disable(){
    if(type) open();
    else close();
  }

  Solenoid(int ena, int oddIn, int evenIn, bool type) : 
  ena(ena), oddIn(oddIn), evenIn(evenIn), type(type), isOpen(type) {}
};
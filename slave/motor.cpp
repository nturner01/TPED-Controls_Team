#include <Arduino.h>
#include "enums.h"
#pragma once

class Motor{
private:
BoardPins board;

bool isForward = false;
bool isReverse = false;

public:
void setup(){
    pinMode(board.leftEna, OUTPUT);
    pinMode(board.rightEna, OUTPUT);
    pinMode(board.in1, OUTPUT);
    pinMode(board.in2, OUTPUT);
    pinMode(board.in3, OUTPUT);
    pinMode(board.in4, OUTPUT);
}

void off() {
    if(isForward || isReverse){
        isForward = false;
        isReverse = false;

        digitalWrite(board.in1, LOW);
        digitalWrite(board.in2, LOW);
        digitalWrite(board.in3, LOW);
        digitalWrite(board.in4, LOW);
        analogWrite(board.leftEna, 255);
        analogWrite(board.rightEna, 255);
    }
}

void forward(){
    if(!isForward){
        isForward = true;
        isReverse = false;

        digitalWrite(board.in1, HIGH);
        digitalWrite(board.in3, HIGH);

        digitalWrite(board.in2, LOW);
        digitalWrite(board.in4, LOW);

        analogWrite(board.leftEna, 255);
        analogWrite(board.rightEna, 255);
    }
}

void reverse(){
    if(!isReverse){
        isReverse = true;
        isForward = false;

        digitalWrite(board.in1, LOW);
        digitalWrite(board.in3, LOW);

        digitalWrite(board.in2, HIGH);
        digitalWrite(board.in4, HIGH);

        analogWrite(board.leftEna, 255);
        analogWrite(board.rightEna, 255);
    }
}

Motor(BoardPins board) {
    this->board = board;
}
};
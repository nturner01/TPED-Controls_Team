#include "actuator.cpp"
#include "motor.cpp"
#pragma once

// all four brakes
int b1Pin = 0;
int b2Pin = 0;
int b3Pin = 0;
int b4Pin = 0;

// motor group 1 (station)
int ena1Pin = 0;
int in11Pin = 0;
int in21Pin = 0;
int a11Pin = 0;
int a21Pin = 0;

// motor group 2 (lift)
int ena2Pin = 0;
int in12Pin = 0;
int in22Pin = 0;
int a12Pin = 0;
int a22Pin = 0;

Actuator brake1(b1Pin);
Actuator brake2(b2Pin);
Actuator brake3(b3Pin);
Actuator brake4(b4Pin);

Motor stationMotors(ena1Pin, in11Pin, in21Pin, a11Pin, a21Pin);
Motor liftMotors(ena2Pin, in12Pin, in22Pin, a12Pin, a22Pin);



#include "enums.h"
#include "solenoid.cpp"
#include "motor.cpp"
#pragma once

#define BRAKE false
#define MOTOR true

// 8 Motors
BoardPins b1 = {8, 7, 32, 33, 34, 35};

// Brake (1) / Motor Actuator (3)
BoardPins b2 = {3, 2, 26, 27, 28, 29};

Motor motors(b1);
Solenoid brakes(b2.leftEna, b2.in1, b2.in2, BRAKE);
Solenoid acts(b2.rightEna, b2.in3, b2.in4, MOTOR);


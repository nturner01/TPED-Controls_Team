#pragma once;
enum FiniteState : uint8_t{
    OFF,
    IDLE, // IDLE BLOCKED
    READY, // IDLE READY
    REVERSE, // REVERSE BLOCKED
    DISPATCHED, // DISPATCHED BLOCKED
    DISPATCHED_2,
    DISPATCHED_3,
    FORWARD, // FORWARD BLOCKED
    IDLE_MAINT, // IDLE MAINTENANCE
    REVERSE_MAINT, // REVERSE MAINTENANCE
    FORWARD_MAINT, // FORWARD MAINTENANCE
    EMERGENCY, // IDLE EMERGENCY
};

struct BoardPins {
  int leftEna;
  int rightEna;
  int in1;
  int in3;
  int in2;
  int in4;
};
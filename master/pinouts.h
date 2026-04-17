#pragma once
// Holds the current operational mode
enum toggleState{
   // CENTER
    AUTOMATIC_MODE,
    NEUTRAL_MODE,

    // RIGHT
    MAINTENANCE_MODE,
    FORWARD_MODE,

    // LEFT
    CONTINUOUS_MODE,
    REVERSE_MODE
};
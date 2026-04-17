#pragma once
// Holds the current operational mode
enum OperationalMode{
    CONTINUOUS_MODE,
    AUTOMATIC_MODE,
    MAINTENANCE_MODE
};

// Holds the direction of the drive selector switch while in maintenance mode
enum MaintenanceMode{ 
    REVERSE_MODE = -1,
    NEUTRAL_MODE,
    FORWARD_MODE
};
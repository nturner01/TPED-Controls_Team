#define LOW 0
#define HIGH 1


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

// Dummy functions for button reading
bool readDispatchButton(){return LOW;}
bool readEStopButton(){return LOW;}
bool readEStopReset(){return LOW;}
OperationalMode readOperationalMode(){return AUTOMATIC_MODE;}
MaintenanceMode readMaintenanceMode(){return NEUTRAL_MODE;}
bool readStationSensor(){return LOW;}
bool readLiftSensor(){return LOW;}
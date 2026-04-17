#define LOW 0
#define HIGH 1




// Dummy functions for button reading
bool readDispatchButton(){return LOW;}
bool readEStopButton(){return LOW;}
bool readEStopReset(){return LOW;}
OperationalMode readOperationalMode(){return AUTOMATIC_MODE;}
MaintenanceMode readMaintenanceMode(){return NEUTRAL_MODE;}
bool readStationSensor(){return LOW;}
bool readLiftSensor(){return LOW;}
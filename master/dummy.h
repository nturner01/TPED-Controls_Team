#include <Arduino.h>
#include "enums.h"
class Motor{
    public:
    void forward(){} // closed by default
    void reverse(){} // closed by default
    void stop(){} // open by default, unless no power
};

class Brake{
    public:
    void open(){}
    void close(){}
};

Motor motors;
Brake brakes;

bool readDispatchButton(){return LOW;}
bool readEStopButton(){return LOW;}
bool readEStopReset(){return LOW;}
ToggleReading readOperationalMode(){return AUTOMATIC_MODE;}
ToggleReading readMaintenanceMode(){return NEUTRAL_MODE;}
bool readFrontSensor(){return LOW;}
bool readBackSensor(){return LOW;}
bool readLiftSensor(){return LOW;}
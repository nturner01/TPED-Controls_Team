/*
  Ride control FSM — single-file sketch (drop this folder into Arduino IDE or paste on GitHub).

  Wiring: one momentary alarm button on PIN_ALARM_BUTTON (INPUT_PULLUP, HIGH released, LOW pressed).
  Same button: trip in normal states; acknowledge in TROUBLE/EMERGENCY after release, then press again.
*/

#include <Arduino.h>

// ---------------------------------------------------------------------------
// Config
// ---------------------------------------------------------------------------

static const unsigned long SERIAL_BAUD = 115200;

#ifndef DEBUG_SERIAL_FSM
#define DEBUG_SERIAL_FSM 1
#endif
#ifndef DEBUG_SERIAL_MOTORS
#define DEBUG_SERIAL_MOTORS 0
#endif

// Pin map — adjust for your board
static const int PIN_ALARM_BUTTON = 2;
static const int PIN_TROUBLE_BUTTON = PIN_ALARM_BUTTON;
static const int PIN_CLEAR_BUTTON = PIN_ALARM_BUTTON;
static const int PIN_SOS_LED = LED_BUILTIN;

static const int PIN_ESTOP = 4;
static const int PIN_ESTOP_RESET = 5;
static const int PIN_DISPATCH = 6;
static const int PIN_STATION_SENSOR = 7;
static const int PIN_LIFT_SENSOR = 8;

#define E_STOP_ACTIVE_LOW 1
#define E_STOP_RESET_ACTIVE_LOW 1

// ---------------------------------------------------------------------------
// Motor / brake (replace with real driver pins as needed)
// ---------------------------------------------------------------------------

#if DEBUG_SERIAL_MOTORS
#define MDBG(x) Serial.println(x)
#else
#define MDBG(x) ((void)0)
#endif

class Motor {
public:
    void reverse();
    void forward();
    void stop();
};

class Break {
public:
    void open();
    void close();
};

void Motor::reverse() {
    MDBG(F("Motors: reverse"));
}

void Motor::forward() {
    MDBG(F("Motors: forward"));
}

void Motor::stop() {
    MDBG(F("Motors: stop"));
}

void Break::open() {
    MDBG(F("Brakes: open"));
}

void Break::close() {
    MDBG(F("Brakes: close"));
}

// ---------------------------------------------------------------------------
// Inputs
// ---------------------------------------------------------------------------

enum OperationalMode {
    CONTINUOUS_MODE,
    AUTOMATIC_MODE,
    MAINTENANCE_MODE
};

enum MaintenanceMode {
    REVERSE_MODE = -1,
    NEUTRAL_MODE,
    FORWARD_MODE
};

void inputsInit() {
    pinMode(PIN_ALARM_BUTTON, INPUT_PULLUP);
    pinMode(PIN_ESTOP, INPUT_PULLUP);
    pinMode(PIN_ESTOP_RESET, INPUT_PULLUP);
    pinMode(PIN_DISPATCH, INPUT_PULLUP);
    pinMode(PIN_STATION_SENSOR, INPUT_PULLUP);
    pinMode(PIN_LIFT_SENSOR, INPUT_PULLUP);

    pinMode(PIN_SOS_LED, OUTPUT);
    digitalWrite(PIN_SOS_LED, LOW);
}

bool readDispatchButton() {
    return digitalRead(PIN_DISPATCH);
}

bool readEStopButton() {
#if E_STOP_ACTIVE_LOW
    return digitalRead(PIN_ESTOP) == LOW ? HIGH : LOW;
#else
    return digitalRead(PIN_ESTOP);
#endif
}

bool readEStopReset() {
#if E_STOP_RESET_ACTIVE_LOW
    return digitalRead(PIN_ESTOP_RESET) == LOW ? HIGH : LOW;
#else
    return digitalRead(PIN_ESTOP_RESET);
#endif
}

OperationalMode readOperationalMode() {
    return AUTOMATIC_MODE;
}

MaintenanceMode readMaintenanceMode() {
    return NEUTRAL_MODE;
}

bool readStationSensor() {
    return digitalRead(PIN_STATION_SENSOR);
}

bool readLiftSensor() {
    return digitalRead(PIN_LIFT_SENSOR);
}

bool readTroubleButton() {
    return digitalRead(PIN_ALARM_BUTTON);
}

bool readClearButton() {
    return digitalRead(PIN_ALARM_BUTTON);
}

// ---------------------------------------------------------------------------
// FSM
// ---------------------------------------------------------------------------

enum CurrentState {
    IDLE,
    READY,
    REVERSE,
    DISPATCHED,
    FORWARD,
    IDLE_MAINT,
    REVERSE_MAINT,
    FORWARD_MAINT,
    EMERGENCY,
    TROUBLE,
};

static Motor motors;
static Break breaks;

static CurrentState state = IDLE;
static CurrentState previousState = IDLE;

static const unsigned long DEBOUNCE_MS = 40;
static bool lastAlarmReading = HIGH;
static bool stableAlarmState = HIGH;
static unsigned long alarmLastChangeMs = 0;
static bool buttonReleasedSinceLastTrip = true;

static const int SOS_UNIT_MS = 200;

#if DEBUG_SERIAL_FSM
#define FDBG(x) Serial.println(x)
#define FDBG_F(x) Serial.println(F(x))
#else
#define FDBG(x) ((void)0)
#define FDBG_F(x) ((void)0)
#endif

#if DEBUG_SERIAL_FSM
static int lastLoggedState = -1;
#endif

static bool alarmButtonLowEdge() {
    bool reading = readTroubleButton();

    if (reading != lastAlarmReading) {
        alarmLastChangeMs = millis();
        lastAlarmReading = reading;
    }

    if ((millis() - alarmLastChangeMs) <= DEBOUNCE_MS)
        return false;

    if (stableAlarmState == reading)
        return false;

    stableAlarmState = reading;
    if (stableAlarmState == HIGH) {
        buttonReleasedSinceLastTrip = true;
        return false;
    }
    return true;
}

static bool sosLedOnAtTime(unsigned long t) {
    const unsigned long u = static_cast<unsigned long>(SOS_UNIT_MS);

#define SOS_SEG(len, ledOn) \
    do {                    \
        if (t < (len))      \
            return (ledOn); \
        t -= (len);         \
    } while (0)

    SOS_SEG(u, true);
    SOS_SEG(u, false);
    SOS_SEG(u, true);
    SOS_SEG(u, false);
    SOS_SEG(u, true);
    SOS_SEG(2 * u, false);

    SOS_SEG(3 * u, true);
    SOS_SEG(u, false);
    SOS_SEG(3 * u, true);
    SOS_SEG(u, false);
    SOS_SEG(3 * u, true);
    SOS_SEG(2 * u, false);

    SOS_SEG(u, true);
    SOS_SEG(u, false);
    SOS_SEG(u, true);
    SOS_SEG(u, false);
    SOS_SEG(u, true);
    SOS_SEG(u, false);

#undef SOS_SEG
    return false;
}

static unsigned long sosPatternLengthMs() {
    const unsigned long u = static_cast<unsigned long>(SOS_UNIT_MS);
    return 7 * u + 13 * u + 6 * u;
}

static void updateSosLed(bool active) {
    static bool lastLed = false;
    static bool cycleInit = false;
    static unsigned long cycleStart = 0;

    if (!active) {
        if (lastLed) {
            digitalWrite(PIN_SOS_LED, LOW);
            lastLed = false;
        }
        cycleInit = false;
        return;
    }

    if (!cycleInit) {
        cycleStart = millis();
        cycleInit = true;
        FDBG_F("TROUBLE: SOS pattern on LED pin");
    }

    unsigned long t = (millis() - cycleStart) % sosPatternLengthMs();
    bool on = sosLedOnAtTime(t);
    if (on != lastLed) {
        digitalWrite(PIN_SOS_LED, on ? HIGH : LOW);
        lastLed = on;
    }
}

static void emergencyStop() {
    motors.stop();
    breaks.close();
}

static void enterTroubleState(CurrentState fromState) {
    previousState = fromState;
    state = TROUBLE;
    emergencyStop();
    FDBG_F("TROUBLE: emergency stop engaged");
}

static void clearTrouble() {
    updateSosLed(false);
    state = previousState;
#if DEBUG_SERIAL_FSM
    lastLoggedState = -1;
#endif
    FDBG_F("TROUBLE: cleared — resuming previous state");
}

static void logStateIfChanged(CurrentState s) {
#if DEBUG_SERIAL_FSM
    if ((int)s == lastLoggedState)
        return;
    lastLoggedState = (int)s;
    switch (s) {
    case IDLE:
        FDBG_F("State: IDLE");
        break;
    case READY:
        FDBG_F("State: READY");
        break;
    case DISPATCHED:
        FDBG_F("State: DISPATCHED");
        break;
    case FORWARD:
        FDBG_F("State: FORWARD");
        break;
    case EMERGENCY:
        FDBG_F("State: EMERGENCY");
        break;
    case TROUBLE:
        FDBG_F("State: TROUBLE");
        break;
    default:
        FDBG_F("State: (other)");
        break;
    }
#else
    (void)s;
#endif
}

void fsmInit() {
    state = IDLE;
    previousState = IDLE;
    lastAlarmReading = readTroubleButton();
    stableAlarmState = lastAlarmReading;
    alarmLastChangeMs = millis();
    buttonReleasedSinceLastTrip = (stableAlarmState == HIGH);
}

void fsmTick() {
    if (alarmButtonLowEdge()) {
        if (state == TROUBLE || state == EMERGENCY) {
            if (buttonReleasedSinceLastTrip) {
                if (state == TROUBLE) {
                    clearTrouble();
                } else {
                    state = IDLE;
                    updateSosLed(false);
                    FDBG_F("EMERGENCY: button acknowledged — IDLE");
                }
                buttonReleasedSinceLastTrip = false;
            }
        } else if (state != TROUBLE && state != EMERGENCY) {
            enterTroubleState(state);
            buttonReleasedSinceLastTrip = false;
        }
    }

    if (state == TROUBLE) {
        emergencyStop();
        updateSosLed(true);
        return;
    }

    if (state != TROUBLE && state != EMERGENCY) {
        updateSosLed(false);
    }

    switch (state) {
    case IDLE: {
        bool estop = readEStopButton();
        bool station = readStationSensor();
        OperationalMode reading = readOperationalMode();

        if (estop == HIGH) {
            state = EMERGENCY;
            break;
        }
        if (reading == MAINTENANCE_MODE) {
            state = IDLE_MAINT;
            break;
        }
        if (station == HIGH) {
            state = READY;
            break;
        }

        breaks.close();
        motors.stop();
        break;
    }

    case READY: {
        bool estop = readEStopButton();
        bool dispatch = readDispatchButton();

        if (estop == HIGH) {
            state = EMERGENCY;
            break;
        }
        if (dispatch == HIGH) {
            state = DISPATCHED;
            break;
        }

        breaks.close();
        motors.stop();
        break;
    }

    case DISPATCHED: {
        bool estop = readEStopButton();
        bool lift = readLiftSensor();

        if (estop == HIGH) {
            state = EMERGENCY;
            break;
        }
        if (lift == HIGH) {
            state = FORWARD;
            breaks.open();
            motors.forward();
            break;
        }

        breaks.close();
        motors.stop();
        break;
    }

    case FORWARD: {
        bool estop = readEStopButton();

        if (estop == HIGH) {
            state = EMERGENCY;
            break;
        }

        breaks.open();
        motors.forward();
        break;
    }

    case EMERGENCY:
        break;

    case REVERSE:
    case IDLE_MAINT:
    case REVERSE_MAINT:
    case FORWARD_MAINT:
        FDBG_F("State: maintenance / reverse — stub");
        break;

    case TROUBLE:
        break;

    default:
        FDBG_F("Invalid state");
        break;
    }

    if (state == EMERGENCY) {
        emergencyStop();
        updateSosLed(true);
        if (readEStopReset() == HIGH) {
            state = IDLE;
            updateSosLed(false);
            FDBG_F("EMERGENCY: E-stop reset — IDLE");
        } else {
            return;
        }
    }

    logStateIfChanged(state);
}

// ---------------------------------------------------------------------------
// Arduino
// ---------------------------------------------------------------------------

void setup() {
    Serial.begin(SERIAL_BAUD);
    inputsInit();
    fsmInit();
    Serial.println(F("Ride FSM ready"));
}

void loop() {
    fsmTick();
}

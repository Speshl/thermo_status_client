#ifndef STATE_H
#define STATE_H

#include "string"
#include <ArduinoJson.h>

#define STATE_LENGTH 14 //number of bytes needed to save and reload state
#define TEMPERATURE_BUFFER_AMOUNT 3 //number of degress to pass target by so we don't turn on and off quickly

#define SENSOR_NAME "Garage_Thermometer"

union IntConversion {
  int intValue;
  uint8_t byteValue[4];
};

struct InternalThermoMeasure {
    int temp;
    int humidity;
    int heatIndex;
};

struct ExternalThermoMeasure {
    int temp;
};

struct State {
	bool enabled;
  bool heatOn;
  bool locallyUpdated;
  InternalThermoMeasure internal;
  ExternalThermoMeasure external;
  int diffTemp;
  int targetDiffTemp;
  int targetTemp;
  int targetBypassOffset;
  InternalThermoMeasure internalToAvg [20];
  ExternalThermoMeasure externalToAvg [20];
  int numMeasuresToAvg;
  int adjustedTarget;
};

void describeState(State* state);

void getStateAsBytes(State* state, uint8_t data[STATE_LENGTH]);

void setStateFromBytes(State* state, uint8_t data[STATE_LENGTH]);

void getStateAsJson(State* state, JsonObject json);

void setStateFromJson(State* state, JsonObject json);

#endif
#include "state.h"

void describeState(State* state){
  Serial.println("----------Describing State----------");

  Serial.print("Enabled: ");
  Serial.println(state->enabled);

  Serial.print("Heat On: ");
  Serial.println(state->heatOn);

  Serial.print("Inside Temp: ");
  Serial.println(state->internal.temp);

  Serial.print("Inside Humidity: ");
  Serial.println(state->internal.humidity);

  Serial.print("Inside HeatIndex: ");
  Serial.println(state->internal.heatIndex);

  Serial.print("Outside Temp: ");
  Serial.println(state->external.temp);

  Serial.print("Diff Temp: ");
  Serial.println(state->diffTemp);

  Serial.print("Target Diff Temp: ");
  Serial.println(state->targetDiffTemp);

  Serial.print("Target Temp: ");
  Serial.println(state->targetTemp);
  Serial.println("------------------------------------");
}

void getStateAsBytes(State* state, uint8_t data[STATE_LENGTH]){
  data[0] = state->enabled;
  data[1] = state->heatOn;

  IntConversion converter;
  converter.intValue = state->targetDiffTemp;
  data[2] = converter.byteValue[3];
  data[3] = converter.byteValue[2];
  data[4] = converter.byteValue[1];
  data[5] = converter.byteValue[0];

  converter.intValue = state->targetTemp;
  data[6] = converter.byteValue[3];
  data[7] = converter.byteValue[2];
  data[8] = converter.byteValue[1];
  data[9] = converter.byteValue[0];

  converter.intValue = state->targetBypassOffset;
  data[10] = converter.byteValue[3];
  data[11] = converter.byteValue[2];
  data[12] = converter.byteValue[1];
  data[13] = converter.byteValue[0];
}

void setStateFromBytes(State* state, uint8_t data[STATE_LENGTH]){
  state->enabled = data[0];
  state->heatOn = data[1];
 
  IntConversion converter;
  converter.byteValue[2] = data[2];
  converter.byteValue[3] = data[3];
  converter.byteValue[4] = data[4];
  converter.byteValue[5] = data[5];
  state->targetDiffTemp = converter.intValue;

  converter.byteValue[2] = data[6];
  converter.byteValue[3] = data[7];
  converter.byteValue[4] = data[8];
  converter.byteValue[5] = data[9];
  state->targetTemp = converter.intValue;

  converter.byteValue[2] = data[10];
  converter.byteValue[3] = data[11];
  converter.byteValue[4] = data[12];
  converter.byteValue[5] = data[13];
  state->targetBypassOffset = converter.intValue;
}

void getStateAsJson(State* state, JsonObject json){
  json["SourceName"] = SENSOR_NAME;
  json["LocallyUpdated"] = state->locallyUpdated;
  json["Enabled"] = state->enabled;
  json["HeatOn"] = state->heatOn;
  json["InsideTemp"] = state->internal.temp;
  json["InsideHumidity"] = state->internal.humidity;
  json["InsideHeatIndex"] = state->internal.heatIndex;
  json["OutsideTemp"] = state->external.temp;
  json["DiffTemp"] = state->diffTemp;
  json["TargetDiffTemp"] = state->targetDiffTemp;
  json["TargetTemp"] = state->targetTemp;
  json["TargetBypassOffset"] = state->targetBypassOffset;
}

void setStateFromJson(State* state, JsonObject json){
  state->enabled = json["Enabled"];
  //state->heatOn = json["heatOn"];
  //state->insideTemp = json["insideTemp"];
  //state->outsideTemp = json["outsideTemp"];
  //state->diffTemp = json["diffTemp"];
  state->targetDiffTemp = json["TargetDiffTemp"];
  state->targetTemp = json["TargetTemp"];
  state->targetBypassOffset = json["TargetBypassOffset"];
}
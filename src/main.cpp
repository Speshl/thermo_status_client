#include <Arduino.h>
#include <storage.h>
#include <state.h>
#include <web.h>
#include <Thermo.h>
#include <display.h>


Preferences preferences;
State state;
DHT dhtInternal(DHTINTERNALPIN, DHTTYPE);
// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

TFT_eSPI    tft = TFT_eSPI();
TFT_eSprite img = TFT_eSprite(&tft);

#define STATEUPDATEINTERVAL 30000
#define TEMPCHECKINTERVAL 10000
#define SYNCINTERVAL 60000
#define TEMPCHECKPERINTERVAL SYNCINTERVAL/TEMPCHECKINTERVAL

unsigned long nextStateUpdate;
unsigned long nextTempCheck;
unsigned long nextSync;

void loadSavedIfFound(State* state){
 bool matchedAll = true; //TEMP SHOULD BE true
  if(isFirstRun(&preferences) == true || matchedAll == false){
    Serial.println("First Run, Using defaults");
    matchedAll = false;
  }else{
    Serial.println("Checking Saved State...");
    uint8_t buffer[STATE_LENGTH];
    if(loadState(&preferences, buffer, STATE_LENGTH)){
      Serial.println("Found Saved State");
      setStateFromBytes(state, buffer);
      //describeState(&currentState->switches);
    }else{
      Serial.println("NO Found Saved Switch State");
      matchedAll = false;
    }
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("Setting Up...");
  initializeThermo(&dhtInternal, &sensors);
  initializeDisplay(&tft);
  initializeStorage(&preferences);
  loadSavedIfFound(&state);
  describeState(&state);
  initializeWIFI();
}

void updateHeaterState(State* state){
  if(!state->enabled){
    return;
  }

  if(state->enabled && state->heatOn){
    Serial.println("Heat ON!");
    //send on command
  }else{
    Serial.println("Heat OFF!");
    //send off command
  }
}

void updateState(State* state){
  state->internal = getAvg(state->internalToAvg, state->numMeasuresToAvg);
  state->external = getAvg(state->externalToAvg, state->numMeasuresToAvg);

  memset(state->internalToAvg, 0, sizeof(state->internalToAvg));
  memset(state->externalToAvg, 0, sizeof(state->externalToAvg));
  state->numMeasuresToAvg = 0;

  state->diffTemp = state->internal.temp - state->external.temp;

  state->heatOn = false; //turn heat off unless turned on

  
  if(state->diffTemp < state->targetDiffTemp){ //still allowable range to increase temp
    int newTarget = (state->targetDiffTemp - state->diffTemp) + state->external.temp;
    if(newTarget > state->targetTemp){
      state->adjustedTarget = state->targetTemp; //set this to the lesser of the target values
    }else{
      state->adjustedTarget = state->targetTemp;
    }

    if(state->internal.temp < state->adjustedTarget + state->targetBypassOffset){
      state->heatOn = true;
      Serial.println("Heat on!");
    }
  }
}

void getThermoState(State* state, DHT* internal, DallasTemperature* external){
  state->internalToAvg[state->numMeasuresToAvg] = getInternalThermoMeasure(internal);
  state->externalToAvg[state->numMeasuresToAvg] = getExternalThermoMeasure(external);
  state->numMeasuresToAvg++;
}

void loop() {
  unsigned long eventTime = millis();

  if(eventTime >= nextTempCheck){
    getThermoState(&state, &dhtInternal, &sensors);
    nextTempCheck = nextTempCheck + TEMPCHECKINTERVAL;
  }

  if(eventTime >= nextStateUpdate){
    updateState(&state);
    updateHeaterState(&state);
    describeState(&state);
    updateDisplay(&tft, &state);
    nextStateUpdate = nextStateUpdate + STATEUPDATEINTERVAL;
  }

  if(eventTime >= nextSync){
    syncWithServer(&state);
    uint8_t data [STATE_LENGTH];
    getStateAsBytes(&state, data);
    saveState(&preferences, data, STATE_LENGTH);
    nextSync = nextSync + SYNCINTERVAL;
  }
}
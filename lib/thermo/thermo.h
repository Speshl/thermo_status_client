#ifndef THERMO_H
#define THERMO_H
#include <state.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "DHT.h"


#define DHTTYPE    DHT22     // DHT 22 (AM2302)
#define DHTINTERNALPIN GPIO_NUM_13
#define DHTEXTERNALPIN GPIO_NUM_14
#define ONE_WIRE_BUS DHTEXTERNALPIN

void initializeThermo(DHT* internal, DallasTemperature* external);

InternalThermoMeasure getInternalThermoMeasure(DHT* thermo);
InternalThermoMeasure getAvg(InternalThermoMeasure measures[], int count);

ExternalThermoMeasure getExternalThermoMeasure(DallasTemperature* thermo);
ExternalThermoMeasure getAvg(ExternalThermoMeasure measures[], int count);

#endif
#include "thermo.h"

void initializeThermo(DHT* internal, DallasTemperature* external){
    internal->begin();
    external->begin();
}

InternalThermoMeasure getInternalThermoMeasure(DHT* thermo){
    InternalThermoMeasure returnValue;

    returnValue.humidity = int(thermo->readHumidity());
    // Read temperature as Fahrenheit (isFahrenheit = true)
    returnValue.temp = int(thermo->readTemperature(true));
    if (isnan(returnValue.humidity) || isnan(returnValue.temp)) {
        Serial.println(F("Failed to read from DHT sensor!"));
        return returnValue;
    }
    returnValue.heatIndex = int(thermo->computeHeatIndex(returnValue.temp, returnValue.humidity));

    return returnValue;
}

InternalThermoMeasure getAvg(InternalThermoMeasure measures[], int count){
    InternalThermoMeasure returnValue;
    int totalTemp = 0;
    int totalHumidity = 0;
    int totalHeatIndex = 0;

    if(count < 1){
        return returnValue;
    }


    for(int i=0; i<count; i++){
        totalTemp += measures[i].temp;
        totalHumidity += measures[i].humidity;
        totalHeatIndex += measures[i].heatIndex;
    }

    returnValue.temp = totalTemp / count;
    returnValue.humidity = totalHumidity / count;
    returnValue.heatIndex = totalHeatIndex / count;
    return returnValue;
}

ExternalThermoMeasure getExternalThermoMeasure(DallasTemperature* thermo){
    ExternalThermoMeasure returnValue;
    thermo->requestTemperatures();
    returnValue.temp = int(thermo->getTempFByIndex(0));
    return returnValue;
}

ExternalThermoMeasure getAvg(ExternalThermoMeasure measures[], int count){
    ExternalThermoMeasure returnValue;
    int totalTemp = 0;

    if(count < 1){
        return returnValue;
    }


    for(int i=0; i<count; i++){
        totalTemp += measures[i].temp;
    }

    returnValue.temp = totalTemp / count;
    return returnValue;
}
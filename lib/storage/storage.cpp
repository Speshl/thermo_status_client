#include "storage.h"

void initializeStorage(Preferences* preferences){
  preferences->begin("Storage", false);
}

bool isFirstRun(Preferences* preferences){
  bool firstRun = preferences->getBool("FirstRun", true);
  if(firstRun == true){
    preferences->putBool("FirstRun", false);
  }
  return firstRun;
}

void saveState(Preferences* preferences, uint8_t value[], int valueSize){
  preferences->putBytes("State", value, valueSize);
}

bool loadState(Preferences* preferences, uint8_t value[], int valueSize){
  int bytesRead = preferences->getBytes("State",value,valueSize);
  if(bytesRead == valueSize){
    return true;
  }else{
    return false;
  }
}

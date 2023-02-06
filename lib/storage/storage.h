#ifndef STORAGE_H
#define STORAGE_H
#include <Preferences.h>

void initializeStorage(Preferences* preferences);

bool isFirstRun(Preferences* preferences);

void saveState(Preferences* preferences, uint8_t value[], int valueSize);

bool loadState(Preferences* preferences, uint8_t value[], int valueSize);

#endif

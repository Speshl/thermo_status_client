#ifndef DISPLAY_H
#define DISPLAY_H
#include <state.h>
#include <TFT_eSPI.h>

#define FF18 &FreeSans12pt7b
#define GFXFF 1

#define IWIDTH  240
#define IHEIGHT 30
#define WAIT 0

void initializeDisplay(TFT_eSPI* tft);
void updateDisplay(TFT_eSPI* tft, State* state);

#endif
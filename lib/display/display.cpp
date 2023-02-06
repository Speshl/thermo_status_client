#include <display.h>

void initializeDisplay(TFT_eSPI* tft){
  tft->init();
  tft->setRotation(3);
  tft->fillScreen(TFT_BLACK);
}

void updateDisplay(TFT_eSPI* tft, State* state){
  tft->setCursor(0,0,2);
  tft->setTextColor(TFT_WHITE, TFT_BLACK);
  tft->setTextSize(2);

  std::string systemStatus;
  if(state->enabled){
    systemStatus = "System        ON";
  }else{
    systemStatus = "System        OFF";
  }

  std::string heatStatus;
  if(state->heatOn){
    heatStatus = "Heat          ON";
  }else{
    heatStatus = "Heat          OFF";
  }
  tft->fillScreen(TFT_BLACK);
  tft->print("Target:         ");
  tft->println(state->targetTemp);

  tft->print("Adj. Target:    ");
  tft->println(state->adjustedTarget);

  tft->print("Inside:         ");
  tft->println(state->internal.temp);

  tft->print("Outside:        ");
  tft->println(state->external.temp);

  tft->print("Bypass:         ");
  tft->println(state->targetBypassOffset);

  tft->println(systemStatus.c_str());
  tft->println(heatStatus.c_str());

}
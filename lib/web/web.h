#ifndef WEB_H
#define WEB_H
#include <state.h>
#include <WiFi.h>
#include <HTTPClient.h>

#define SSID "1.21_Jiggawatts"
#define WIFIPASSWORD "redalert"
#define SERVERENDPOINT "http://192.168.1.22:8080/thermo/sync"

void initializeWIFI();

void syncWithServer(State* state);

#endif
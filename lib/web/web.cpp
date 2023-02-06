#include <web.h>

void initializeWIFI(){
  WiFi.begin(SSID, WIFIPASSWORD);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void syncWithServer(State* state){
  if(WiFi.status()== WL_CONNECTED){
    WiFiClient client;
    HTTPClient http;

    char requestBody[500];
    StaticJsonDocument<500> requestDoc;
    JsonObject obj = requestDoc.to<JsonObject>();

    getStateAsJson(state, obj);
    serializeJson(obj, requestBody);

    // Your Domain name with URL path or IP address with path
    http.begin(client, SERVERENDPOINT);
    
    // If you need an HTTP request with a content type: application/json, use the following:
    http.addHeader("Content-Type", "application/json");
    int httpResponseCode = http.POST(requestBody);
    
    if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);

      std::string payload = http.getString().c_str();
      StaticJsonDocument<500> responseDoc;
      DeserializationError error = deserializeJson(responseDoc, payload);

      if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return;
      }

      JsonObject object = responseDoc.as<JsonObject>();
      setStateFromJson(state, object);
      Serial.println("state synced with server");
      describeState(state);
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    // Free resources
    http.end();
  }
  else {
    Serial.println("WiFi Disconnected");
  }
}
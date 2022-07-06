#include "ble.h"
#include "infos.h"
#include <Arduino.h>
#include <ArduinoJson.h>

void setup() {
  Serial.begin(115200);
  InitBLE();
}

void loop() {
  if (_BLEClientConnected) {
    float temp = getCoreTemperature();
    StaticJsonDocument<200> doc;
    JsonObject telemetry = doc.createNestedObject("telemetry");
    telemetry["coreTemperature"] = temp;
    String output;
    serializeJson(doc, output);
    MainCharacteristic.setValue(output.c_str());
    MainCharacteristic.notify();
    delay(2000);
  }
}
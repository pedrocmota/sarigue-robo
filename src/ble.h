#include <Arduino.h>
#include <ArduinoJson.h>
#include <BLE2902.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>

bool _BLEClientConnected = false;

#define BatteryService BLEUUID((uint16_t)0x180F)
BLECharacteristic MainCharacteristic(BLEUUID((uint16_t)0x2A19), BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
BLEDescriptor MainCharacteristicDescriptor(BLEUUID((uint16_t)0x2901));

class MyServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer *pServer) {
    _BLEClientConnected = true;
  };
  void onDisconnect(BLEServer *pServer) {
    _BLEClientConnected = false;
    pServer->startAdvertising();
  }
};

void InitBLE() {
  BLEDevice::init("Sariguê");
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  BLEService *pService = pServer->createService(BatteryService);
  pService->addCharacteristic(&MainCharacteristic);
  MainCharacteristicDescriptor.setValue("Sarigue - Robo");
  MainCharacteristic.addDescriptor(&MainCharacteristicDescriptor);
  MainCharacteristic.addDescriptor(new BLE2902());

  pServer->getAdvertising()->addServiceUUID(BatteryService);
  pService->start();
  pServer->getAdvertising()->start();
}
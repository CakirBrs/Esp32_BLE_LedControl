#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"


class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string value = pCharacteristic->getValue();

      if (value.length() > 0) {
        Serial.println("*********");
        Serial.print("New value: ");
        for (int i = 0; i < value.length(); i++)
          Serial.print(value[i]);

        Serial.println();
        
        
        if(value[0]=='1'){
          digitalWrite (27, HIGH);
          Serial.println("Led On");

        } 
        if(value[0]=='2'){
          digitalWrite (27, LOW);
          Serial.println("Led Off");

        } 
        Serial.println("*********");
        
      }
    }
};

void setup() {
  Serial.begin(115200);
  pinMode (27, OUTPUT);
  Serial.println("Connect to ESP32BLE");

  BLEDevice::init("ESP32BLE");
  BLEServer *pServer = BLEDevice::createServer();

  BLEService *pService = pServer->createService(SERVICE_UUID);

  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );

  pCharacteristic->setCallbacks(new MyCallbacks());

  pCharacteristic->setValue("Led on:1 - Led off:2");
  pService->start();

  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->start();

  Serial.print("Server address:");
  Serial.println(BLEDevice::getAddress().toString().c_str());
}

void loop() {
  delay(2000);
}
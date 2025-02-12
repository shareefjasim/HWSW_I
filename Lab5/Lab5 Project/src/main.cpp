#include <Arduino.h>
#include <NimBLEDevice.h>

NimBLECharacteristic* pCharacteristic    = nullptr;
bool                  deviceConnected    = false;
bool                  oldDeviceConnected = false;
uint32_t              value              = 0;

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

class ServerCallbacks : public NimBLEServerCallbacks {
    void onConnect(NimBLEServer* pServer, NimBLEConnInfo& connInfo) override {
        deviceConnected = true;
    }

    void onDisconnect(NimBLEServer* pServer, NimBLEConnInfo& connInfo, int reason) override {
        // Peer disconnected, add them to the whitelist
        NimBLEDevice::whiteListAdd(connInfo.getAddress());
        deviceConnected = false;
    }
} serverCallbacks;

void onAdvComplete(NimBLEAdvertising* pAdvertising) {
    Serial.println("Advertising stopped");
    if (deviceConnected) {
        return;
    }
    // If advertising timed out without connection,
    // start again but without whitelist filtering
    pAdvertising->setScanFilter(false, false);
    pAdvertising->start();
}

void setup() {
    Serial.begin(115200);

    NimBLEDevice::init("Whitelist NimBLEServer");

    NimBLEServer* pServer = NimBLEDevice::createServer();
    pServer->setCallbacks(&serverCallbacks);

    // Don't automatically advertise on disconnect,
    // we’ll manage that ourselves
    pServer->advertiseOnDisconnect(false);

    // Create service & characteristic
    NimBLEService* pService = pServer->createService(SERVICE_UUID);

    pCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_UUID,
        NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE | NIMBLE_PROPERTY::NOTIFY
    );

    pService->start();

    // Configure advertising
    NimBLEAdvertising* pAdvertising = NimBLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->enableScanResponse(false);
    pAdvertising->setAdvertisingCompleteCallback(onAdvComplete);

    // Start advertising now
    pAdvertising->start();

    Serial.println("Waiting for a client connection to notify...");
}

void loop() {
    // If a device is connected, periodically notify it with `value`
    if (deviceConnected) {
        pCharacteristic->setValue((uint8_t*)&value, 4);
        pCharacteristic->notify();
        value++;
    }

    // If we were connected, but the client disconnected:
    if (!deviceConnected && oldDeviceConnected) {
        NimBLEAdvertising* pAdvertising = NimBLEDevice::getAdvertising();

        // If there's at least one device in the whitelist,
        // only whitelisted devices can connect
        if (NimBLEDevice::getWhiteListCount() > 0) {
            // allow scanning by all, but only whitelisted devices can connect
            pAdvertising->setScanFilter(false, true);
        }

        // Advertise again (with whitelist) for 30 seconds
        pAdvertising->start(30 * 1000);
        Serial.println("start advertising");
        oldDeviceConnected = deviceConnected;
    }

    // If we just connected, do any “on-connect” logic here
    if (deviceConnected && !oldDeviceConnected) {
        oldDeviceConnected = deviceConnected;
    }

    delay(2000);
}

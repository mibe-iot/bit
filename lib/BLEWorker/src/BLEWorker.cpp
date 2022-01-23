#include <BLEWorker.h>

#include <secrets.h>
#include <BLECallbacks.h>
#include <BLENotifier.h>

uint8_t txValue = 0;

[[noreturn]] void notifierTask(void *param) {
    BLECharacteristic *pTxCharacteristic = (BLECharacteristic *)param;
    while (true) {
        pTxCharacteristic->setValue(&txValue, 1);
        pTxCharacteristic->notify();
        txValue++;
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

void BLEWorker::TaskHandler(void *param) {
    EventGroupHandle_t flags = (EventGroupHandle_t)param;

    BLEDevice::init(Secrets::GetBLEServiceName());

    BLEServer *pServer = BLEDevice::createServer();
    pServer->setCallbacks(new BLECallbacks());

    BLEService *pService = pServer->createService(Secrets::GetBLEServiceUUID());

    BLECharacteristic *pTxCharacteristic = pService->createCharacteristic(
            CHARACTERISTIC_UUID_TX,
            BLECharacteristic::PROPERTY_NOTIFY);
    pTxCharacteristic->addDescriptor(new BLE2902());

    BLECharacteristic *pRxCharacteristic = pService->createCharacteristic(
            CHARACTERISTIC_UUID_RX,
            BLECharacteristic::PROPERTY_WRITE);
    pRxCharacteristic->setCallbacks(new BLENotifier());

    pService->start();
    pServer->getAdvertising()->start();

    xTaskCreate(notifierTask, "notifier", 4096, pTxCharacteristic, 2, nullptr);
    vTaskDelete(nullptr);
}
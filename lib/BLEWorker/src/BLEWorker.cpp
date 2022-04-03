#include <BLEWorker.h>

#include <SharedState.h>
#include <BLECallbacks.h>
#include <IdentifierNotifier.h>
#include <SSIDNotifier.h>
#include <PasswordNotifier.h>

void BLEWorker::TaskHandler(void *param) {
    SharedState *state = (SharedState *) param;

    BLEDevice::init(state->configuration->ble->GetServiceName());

    BLEServer *pServer = BLEDevice::createServer();
    pServer->setCallbacks(new BLECallbacks(state));

    BLEService *pService = pServer->createService(state->configuration->ble->GetServiceReceiver());

    BLECharacteristic *nameReceiverCharacteristic = pService->createCharacteristic(
            state->configuration->ble->GetIdentifierReceiver(),
            BLECharacteristic::PROPERTY_WRITE_NR);
    nameReceiverCharacteristic->setCallbacks(new IdentifierNotifier(state));

    BLECharacteristic *ssidReceiverCharacteristic = pService->createCharacteristic(
            state->configuration->ble->GetSSIDReceiver(),
            BLECharacteristic::PROPERTY_WRITE_NR);
    ssidReceiverCharacteristic->setCallbacks(new SSIDNotifier(state));

    BLECharacteristic *passwordReceiverCharacteristic = pService->createCharacteristic(
            state->configuration->ble->GetPasswordReceiver(),
            BLECharacteristic::PROPERTY_WRITE_NR);
    passwordReceiverCharacteristic->setCallbacks(new PasswordNotifier(state));

    pService->start();

    auto adv = pServer->getAdvertising();
    adv->addServiceUUID(state->configuration->ble->GetServiceReceiver());
    adv->start();

    vTaskDelete(nullptr);
}
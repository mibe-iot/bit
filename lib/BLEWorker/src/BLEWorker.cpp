#include <BLEWorker.h>

#include <secrets.h>
#include <BLECallbacks.h>
#include <NameNotifier.h>
#include <SSIDNotifier.h>
#include <PasswordNotifier.h>

void BLEWorker::TaskHandler(void *param) {
    EventGroupHandle_t flags = (EventGroupHandle_t) param;

    auto secrets = Secrets::GetJsonObject();

    BLEDevice::init(secrets[F("SERVICE_NAME")]);

    BLEServer *pServer = BLEDevice::createServer();
    pServer->setCallbacks(new BLECallbacks(flags));

    BLEService *pService = pServer->createService(secrets[F("SERVICE_UUID")].as<std::string>());

    BLECharacteristic *nameReceiverCharacteristic = pService->createCharacteristic(
            secrets[F("SERVICE_NAME_RECEIVER")].as<std::string>(),
            BLECharacteristic::PROPERTY_WRITE_NR);
    nameReceiverCharacteristic->setCallbacks(new NameNotifier(flags));

    BLECharacteristic *ssidReceiverCharacteristic = pService->createCharacteristic(
            secrets[F("SERVICE_SSID_RECEIVER")].as<std::string>(),
            BLECharacteristic::PROPERTY_WRITE_NR);
    ssidReceiverCharacteristic->setCallbacks(new SSIDNotifier(flags));

    BLECharacteristic *passwordReceiverCharacteristic = pService->createCharacteristic(
            secrets[F("SERVICE_PASSWORD_RECEIVER")].as<std::string>(),
            BLECharacteristic::PROPERTY_WRITE_NR);
    passwordReceiverCharacteristic->setCallbacks(new PasswordNotifier(flags));

    pService->start();
    pServer->getAdvertising()->start();

    vTaskDelete(nullptr);
}
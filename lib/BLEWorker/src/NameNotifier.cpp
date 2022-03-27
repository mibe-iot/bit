#include <NameNotifier.h>
#include <BLEWorker.h>
#include <secrets.h>
#include <Arduino.h>

NameNotifier::NameNotifier(EventGroupHandle_t flags) {
    this->flags = flags;
}

void NameNotifier::onWrite(BLECharacteristic *pCharacteristic) {
    std::string rxValue = pCharacteristic->getValue();

    if (rxValue.length() > 0) {
        xEventGroupSetBits(flags, SharedConnectivityState::NAME_RECEIVED);
    }
}
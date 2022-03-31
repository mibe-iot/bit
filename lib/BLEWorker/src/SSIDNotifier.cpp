#include <BLEWorker.h>
#include <secrets.h>
#include <SSIDNotifier.h>

SSIDNotifier::SSIDNotifier(EventGroupHandle_t flags) {
    this->flags = flags;
}

void SSIDNotifier::onWrite(BLECharacteristic *pCharacteristic) {
    std::string rxValue = pCharacteristic->getValue();

    if (rxValue.length() > 0) {
        xEventGroupSetBits(flags, SharedConnectivityState::SSID_RECEIVED);
        Secrets::SetSSID(rxValue);
    }
}
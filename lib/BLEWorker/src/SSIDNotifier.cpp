#include <BLEWorker.h>
#include <SSIDNotifier.h>

SSIDNotifier::SSIDNotifier(SharedState *state) {
    this->state = state;
}

void SSIDNotifier::onWrite(BLECharacteristic *pCharacteristic) {
    std::string rxValue = pCharacteristic->getValue();

    if (rxValue.length() > 0) {
        xEventGroupSetBits(state->flags, SharedConnectivityState::SSID_RECEIVED);
        state->configuration->wifi->SetSSID(rxValue.c_str());
    }
}
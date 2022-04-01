#include <IdentifierNotifier.h>
#include <BLEWorker.h>

IdentifierNotifier::IdentifierNotifier(SharedState *state) {
    this->state = state;
}

void IdentifierNotifier::onWrite(BLECharacteristic *pCharacteristic) {
    std::string rxValue = pCharacteristic->getValue();

    if (rxValue.length() > 0) {
        xEventGroupSetBits(state->flags, SharedConnectivityState::IDENTIFIER_RECEIVED);
        state->configuration->mqtt->SetDeviceIdentifier(rxValue.c_str());
    }
}
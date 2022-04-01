#include <BLEWorker.h>
#include <PasswordNotifier.h>

PasswordNotifier::PasswordNotifier(SharedState *state) {
    this->state = state;
}

void PasswordNotifier::onWrite(BLECharacteristic *pCharacteristic) {
    std::string rxValue = pCharacteristic->getValue();

    if (rxValue.length() > 0) {
        xEventGroupSetBits(state->flags, SharedConnectivityState::PASSWORD_RECEIVED);
        state->configuration->wifi->SetPassword(rxValue.c_str());
    }
}
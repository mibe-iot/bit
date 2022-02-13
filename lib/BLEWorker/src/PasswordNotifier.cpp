#include <BLEWorker.h>
#include <PasswordNotifier.h>
#include <secrets.h>

PasswordNotifier::PasswordNotifier(EventGroupHandle_t flags) {
    this->flags = flags;
}

void PasswordNotifier::onWrite(BLECharacteristic *pCharacteristic) {
    std::string rxValue = pCharacteristic->getValue();

    if (rxValue.length() > 0) {
        xEventGroupSetBits(flags, BLEState::PASSWORD_RECEIVED);
        Secrets::SetPassword((char *) pCharacteristic->getData());
    }
}
#include <NameNotifier.h>
#include <BLEWorker.h>
#include <Arduino.h>

NameNotifier::NameNotifier(EventGroupHandle_t flags) {
    this->flags = flags;
}

void NameNotifier::onWrite(BLECharacteristic *pCharacteristic) {
    std::string rxValue = pCharacteristic->getValue();

    if (rxValue.length() > 0) {
        xEventGroupSetBits(flags, BLEState::NAME_RECEIVED);

        Serial.println("*********");
        Serial.print("Received Value Name: ");
        for (int i = 0; i < rxValue.length(); i++)
            Serial.print(rxValue[i]);

        Serial.println();
        Serial.println("*********");
    }
}
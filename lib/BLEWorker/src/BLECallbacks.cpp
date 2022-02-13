#include <Arduino.h>
#include <BLECallbacks.h>

BLECallbacks::BLECallbacks(EventGroupHandle_t flags) {
    xEventGroupClearBits(flags, BLEState::CONNECTED);
    this->flags = flags;
}

void BLECallbacks::onConnect(BLEServer *pServer) {
    xEventGroupSetBits(flags, BLEState::CONNECTED);
    Serial.println("Thinker connected");
    BLEServerCallbacks::onConnect(pServer);
}

void BLECallbacks::onDisconnect(BLEServer *pServer) {
    xEventGroupClearBits(flags, BLEState::CONNECTED);
    Serial.println("Thinker disconnected");
    BLEServerCallbacks::onDisconnect(pServer);
}

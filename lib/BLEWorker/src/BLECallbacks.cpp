#include <Arduino.h>
#include <BLECallbacks.h>
#include <secrets.h>

BLECallbacks::BLECallbacks(EventGroupHandle_t flags) {
    xEventGroupClearBits(flags, SharedConnectivityState::CONNECTED);
    this->flags = flags;
}

void BLECallbacks::onConnect(BLEServer *pServer) {
    xEventGroupSetBits(flags, SharedConnectivityState::CONNECTED);
    Serial.println("Thinker connected");
    BLEServerCallbacks::onConnect(pServer);
}

void BLECallbacks::onDisconnect(BLEServer *pServer) {
    xEventGroupClearBits(flags, SharedConnectivityState::CONNECTED);
    Serial.println("Thinker disconnected");
    BLEServerCallbacks::onDisconnect(pServer);
}

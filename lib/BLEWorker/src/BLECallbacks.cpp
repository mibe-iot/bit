#include <Arduino.h>
#include <BLECallbacks.h>

BLECallbacks::BLECallbacks(SharedState *state) {
    Serial.println("XEvent called");
    xEventGroupClearBits(state->flags, SharedConnectivityState::BLE_CONNECTED);
    this->state = state;
}

void BLECallbacks::onConnect(BLEServer *pServer) {
    xEventGroupSetBits(state->flags, SharedConnectivityState::BLE_CONNECTED);
    Serial.println("Thinker connected");
    BLEServerCallbacks::onConnect(pServer);
}

void BLECallbacks::onDisconnect(BLEServer *pServer) {
    xEventGroupClearBits(state->flags, SharedConnectivityState::BLE_CONNECTED);
    Serial.println("Thinker disconnected");
    BLEServerCallbacks::onDisconnect(pServer);
}

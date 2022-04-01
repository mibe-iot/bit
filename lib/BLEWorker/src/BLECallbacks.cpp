#include <BLECallbacks.h>

BLECallbacks::BLECallbacks(SharedState *state) {
    xEventGroupClearBits(state->flags, SharedConnectivityState::BLE_CONNECTED);
    this->state = state;
}

void BLECallbacks::onConnect(BLEServer *pServer) {
    xEventGroupSetBits(state->flags, SharedConnectivityState::BLE_CONNECTED);
    BLEServerCallbacks::onConnect(pServer);
}

void BLECallbacks::onDisconnect(BLEServer *pServer) {
    xEventGroupClearBits(state->flags, SharedConnectivityState::BLE_CONNECTED);
    BLEServerCallbacks::onDisconnect(pServer);
}

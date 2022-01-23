#include <BLECallbacks.h>

BLECallbacks::BLECallbacks(EventGroupHandle_t flags) {
    xEventGroupClearBits(flags, BLEConnected);
    this->flags = flags;
}

void BLECallbacks::onConnect(BLEServer *pServer) {
    xEventGroupSetBits(flags, BLEConnected);
    BLEServerCallbacks::onConnect(pServer);
}

void BLECallbacks::onDisconnect(BLEServer *pServer) {
    xEventGroupClearBits(flags, BLEConnected);
    BLEServerCallbacks::onDisconnect(pServer);
}

#include <BLECallbacks.h>

void BLECallbacks::onConnect(BLEServer *pServer) {
    BLEServerCallbacks::onConnect(pServer);
}

void BLECallbacks::onDisconnect(BLEServer *pServer) {
    BLEServerCallbacks::onDisconnect(pServer);
}

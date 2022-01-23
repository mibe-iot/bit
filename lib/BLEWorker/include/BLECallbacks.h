#pragma once

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

class BLECallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer *pServer) override;
    void onDisconnect(BLEServer *pServer) override;
};
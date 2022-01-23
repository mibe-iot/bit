#pragma once

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

class BLENotifier : public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) override;
};

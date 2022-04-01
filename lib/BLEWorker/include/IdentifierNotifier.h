#pragma once

#include <FreeRTOS.h>
#include <freertos/event_groups.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <SharedState.h>

class IdentifierNotifier : public BLECharacteristicCallbacks {
public:
    IdentifierNotifier(SharedState *state);

public:
    void onWrite(BLECharacteristic *pCharacteristic) override;

private:
    SharedState *state;
};

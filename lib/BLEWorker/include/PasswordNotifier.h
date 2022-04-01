#pragma once

#include <FreeRTOS.h>
#include <freertos/event_groups.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <SharedState.h>

class PasswordNotifier : public BLECharacteristicCallbacks {
public:
    PasswordNotifier(SharedState *state);

public:
    void onWrite(BLECharacteristic *pCharacteristic) override;

private:
    SharedState *state;
};

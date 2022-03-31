#pragma once

#include <FreeRTOS.h>
#include <freertos/event_groups.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

class PasswordNotifier : public BLECharacteristicCallbacks {
public:
    PasswordNotifier(EventGroupHandle_t flags);

public:
    void onWrite(BLECharacteristic *pCharacteristic) override;

private:
    EventGroupHandle_t flags;
};

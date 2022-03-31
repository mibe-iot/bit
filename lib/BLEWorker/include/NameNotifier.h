#pragma once

#include <FreeRTOS.h>
#include <freertos/event_groups.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

class NameNotifier : public BLECharacteristicCallbacks {
public:
    NameNotifier(EventGroupHandle_t flags);

public:
    void onWrite(BLECharacteristic *pCharacteristic) override;

private:
    EventGroupHandle_t flags;
};

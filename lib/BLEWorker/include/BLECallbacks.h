#pragma once

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <freertos/event_groups.h>
#include <BLEWorker.h>

class BLECallbacks : public BLEServerCallbacks {
public:
    explicit BLECallbacks(EventGroupHandle_t flags);

private:
    void onConnect(BLEServer *pServer) override;
    void onDisconnect(BLEServer *pServer) override;

private:
    EventGroupHandle_t flags;
};
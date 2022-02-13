#pragma once

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

enum BLEState : uint32_t {
    CONNECTED = 1 << 0,
    NAME_RECEIVED = 1 << 1,
    SSID_RECEIVED = 1 << 2,
    PASSWORD_RECEIVED = 1 << 3,
};

class BLEWorker {
public:
    static void TaskHandler(void *param);
};
#pragma once

#include <Arduino.h>

const uint8_t WifiConnected = 1 << 0;
const uint32_t WifiTimeout = 30000;

class WiFiWorker {
public:
    [[noreturn]] static void TaskHandler(void *param);
};
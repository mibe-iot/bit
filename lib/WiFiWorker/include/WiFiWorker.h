#pragma once

#include <cstdint>

const uint32_t WifiTimeout = 30000;

class WiFiWorker {
public:
    [[noreturn]] static void Connect(void *param);
    [[noreturn]] static void TaskHandler(void *param);
};
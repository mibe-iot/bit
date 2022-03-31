#pragma once

#include <Arduino.h>

const uint32_t MQTT_TIMEOUT = 30000;
const uint16_t MQTT_PORT = 1883;
const char MQTT_SERVER[] = "thinker.local";
const char MQTT_CLIENT[] = "Bit";

class MQTTWorker {
public:
    [[noreturn]] static void TaskHandler(void *param);
};
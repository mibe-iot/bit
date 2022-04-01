#pragma once

#include <Arduino.h>
#include <SharedConfiguration.h>

enum SharedConnectivityState : uint32_t {
    BLE_CONNECTED = 1 << 0,
    SSID_RECEIVED = 1 << 1,
    PASSWORD_RECEIVED = 1 << 2,
    IDENTIFIER_RECEIVED = 1 << 3,
    WIFI_CONNECTED = 1 << 4,
    MQTT_CONNECT = 1 << 5,
};

struct SharedState {
public:
    void CheckConfiguration() {
        if (configuration->wifi->GetSSID().length()) {
            xEventGroupSetBits(flags, SharedConnectivityState::SSID_RECEIVED);
        }
        if (configuration->wifi->GetPassword().length()) {
            xEventGroupSetBits(flags, SharedConnectivityState::PASSWORD_RECEIVED);
        }
        if (configuration->wifi->GetIdentifier().length()) {
            xEventGroupSetBits(flags, SharedConnectivityState::IDENTIFIER_RECEIVED);
        }
    };

public:
    EventGroupHandle_t flags;
    SharedConfiguration *configuration;
};

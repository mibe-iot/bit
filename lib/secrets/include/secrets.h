#pragma once

#include <string>
#include <ArduinoJson.h>

enum SharedConnectivityState : uint32_t {
    CONNECTED = 1 << 0,
    NAME_RECEIVED = 1 << 1,
    SSID_RECEIVED = 1 << 2,
    PASSWORD_RECEIVED = 1 << 3,
    WIFI_CONNECTED = 1 << 4,
    MQTT_CONNECT = 1 << 5,
};

class Secrets {
public:
    static std::string GetBLEServiceName();

    static std::string GetBLEServiceUUID();

    static std::string GetWiFiSSID();

    static std::string GetWiFiPassword();

    static void SetSSID(std::string _ssid);

    static std::string GetSSID();

    static void SetPassword(std::string _password);

    static std::string GetPassword();

    static std::string GetName();

    static void SetName(std::string _name);

    static JsonObject GetJsonObject();
};
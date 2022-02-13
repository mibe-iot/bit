#pragma once

#include <string>
#include <ArduinoJson.h>

class Secrets
{
public:
    static std::string GetBLEServiceName();
    static std::string GetBLEServiceUUID();
    static std::string GetWiFiSSID();
    static std::string GetWiFiPassword();

    static void SetSSID(const char *_ssid);
    static std::string GetSSID();

    static void SetPassword(const char *_password);
    static std::string GetPassword();

    static JsonObject GetJsonObject();
};
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
    static JsonObject GetJsonObject();
};
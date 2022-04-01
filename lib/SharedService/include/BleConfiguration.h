#pragma once
#include <ArduinoJson.h>

class BleConfiguration {
public:
    BleConfiguration();
    ~BleConfiguration();

public:
    std::string GetServiceName();
    std::string GetServiceReceiver();
    std::string GetIdentifierReceiver();
    std::string GetSSIDReceiver();
    std::string GetPasswordReceiver();

private:
    DynamicJsonDocument *document;
};
#pragma once

#include <Preferences.h>

class MQTTConfiguration {
public:
    MQTTConfiguration();
    ~MQTTConfiguration();

public:
    String GetDeviceIdentifier();
    void SetDeviceIdentifier(String identifier);

private:
    Preferences *preferences;
};
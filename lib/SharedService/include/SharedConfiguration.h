#pragma once

#include <BleConfiguration.h>
#include <WifiConfiguration.h>
#include <MQTTConfiguration.h>

class SharedConfiguration {
public:
    SharedConfiguration();
    ~SharedConfiguration();

public:
    BleConfiguration *ble;
    WifiConfiguration *wifi;
    MQTTConfiguration *mqtt;
};
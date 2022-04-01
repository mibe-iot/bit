#pragma once

#include <BleConfiguration.h>
#include <WifiConfiguration.h>

class SharedConfiguration {
public:
    SharedConfiguration();
    ~SharedConfiguration();

public:
    BleConfiguration *ble;
    WifiConfiguration *wifi;
};
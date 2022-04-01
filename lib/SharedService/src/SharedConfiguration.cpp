#include <SharedConfiguration.h>

SharedConfiguration::SharedConfiguration() {
    ble = new BleConfiguration();
    wifi = new WifiConfiguration();
}

SharedConfiguration::~SharedConfiguration() {
    delete ble;
    delete wifi;
}
#include <SharedConfiguration.h>

SharedConfiguration::SharedConfiguration() {
    ble = new BleConfiguration();
    wifi = new WifiConfiguration();
    mqtt = new MQTTConfiguration();
}

SharedConfiguration::~SharedConfiguration() {
    delete ble;
    delete wifi;
    delete mqtt;
}
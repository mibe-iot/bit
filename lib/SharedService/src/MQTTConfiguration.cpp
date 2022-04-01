#include <MQTTConfiguration.h>
#include <mdns.h>

MQTTConfiguration::MQTTConfiguration() {
    preferences = new Preferences;
    preferences->begin("mqtt");
}

MQTTConfiguration::~MQTTConfiguration() {
    preferences->end();
}

String MQTTConfiguration::GetDeviceIdentifier() {
    return preferences->getString("identifier", "");
}

void MQTTConfiguration::SetDeviceIdentifier(String identifier) {
    preferences->putString("identifier", identifier);
}

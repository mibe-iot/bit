#include <WifiConfiguration.h>
#include <Preferences.h>

WifiConfiguration::WifiConfiguration() {
    preferences = new Preferences;
    preferences->begin("wifi", false);
}

WifiConfiguration::~WifiConfiguration() {
    preferences->end();
}

String WifiConfiguration::GetSSID() {
    return preferences->getString("ssid", "");
}

void WifiConfiguration::SetSSID(String ssid) {
    preferences->putString("ssid", ssid);
}

String WifiConfiguration::GetPassword() {
    return preferences->getString("password", "");
}

void WifiConfiguration::SetPassword(String password) {
    preferences->putString("password", password);
}

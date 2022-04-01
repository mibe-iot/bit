#pragma once

#include <Preferences.h>

class WifiConfiguration {
public:
    WifiConfiguration();
    ~WifiConfiguration();

public:
    String GetSSID();
    void SetSSID(String ssid);

    String GetPassword();
    void SetPassword(String password);

private:
    Preferences *preferences;
};
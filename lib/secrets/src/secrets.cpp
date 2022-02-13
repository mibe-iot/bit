#include <secrets.h>

extern const uint8_t secrets_json_start[] asm("_binary_secrets_json_start");
extern const uint8_t secrets_json_end[] asm("_binary_secrets_json_end");

DynamicJsonDocument doc(1024);

static std::string ssid;
static std::string password;

JsonObject Secrets::GetJsonObject() {
    deserializeJson(doc, secrets_json_start);
    return doc.as<JsonObject>();
}

void Secrets::SetSSID(const char *_ssid) {
    ssid = _ssid;
}

std::string Secrets::GetSSID() {
    return ssid;
}

std::string Secrets::GetBLEServiceName() {
    deserializeJson(doc, secrets_json_start);
    JsonObject obj = doc.as<JsonObject>();
    return obj[F("SERVICE_NAME")];
}

std::string Secrets::GetBLEServiceUUID() {
    deserializeJson(doc, secrets_json_start);
    JsonObject obj = doc.as<JsonObject>();
    return obj[F("SERVICE_UUID")];
}

std::string Secrets::GetWiFiSSID() {
    deserializeJson(doc, secrets_json_start);
    JsonObject obj = doc.as<JsonObject>();
    return obj[F("WIFI_SSID")];
}

std::string Secrets::GetWiFiPassword() {
    deserializeJson(doc, secrets_json_start);
    JsonObject obj = doc.as<JsonObject>();
    return obj[F("WIFI_PASSWORD")];
}

void Secrets::SetPassword(const char *_password) {
    password = _password;
}

std::string Secrets::GetPassword() {
    return password;
}

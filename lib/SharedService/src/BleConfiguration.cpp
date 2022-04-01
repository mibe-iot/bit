#include <BleConfiguration.h>

extern const uint8_t secrets_json_start[] asm("_binary_secrets_json_start");
extern const uint8_t secrets_json_end[] asm("_binary_secrets_json_end");

BleConfiguration::BleConfiguration() {
    document = new DynamicJsonDocument(1024);
}

BleConfiguration::~BleConfiguration() {
    delete document;
}

std::string BleConfiguration::GetServiceName() {
    auto doc = *document;
    deserializeJson(doc, secrets_json_start);
    JsonObject obj = doc.as<JsonObject>();
    return obj[F("SERVICE_NAME")].as<std::string>();
}

std::string BleConfiguration::GetServiceReceiver() {
    auto doc = *document;
    deserializeJson(doc, secrets_json_start);
    JsonObject obj = doc.as<JsonObject>();
    return obj[F("SERVICE_UUID")].as<std::string>();
}

std::string BleConfiguration::GetIdentifierReceiver() {
    auto doc = *document;
    deserializeJson(doc, secrets_json_start);
    JsonObject obj = doc.as<JsonObject>();
    return obj[F("SERVICE_NAME_RECEIVER")].as<std::string>();
}

std::string BleConfiguration::GetSSIDReceiver() {
    auto doc = *document;
    deserializeJson(doc, secrets_json_start);
    JsonObject obj = doc.as<JsonObject>();
    return obj[F("SERVICE_SSID_RECEIVER")].as<std::string>();
}

std::string BleConfiguration::GetPasswordReceiver() {
    auto doc = *document;
    deserializeJson(doc, secrets_json_start);
    JsonObject obj = doc.as<JsonObject>();
    return obj[F("SERVICE_PASSWORD_RECEIVER")].as<std::string>();
}

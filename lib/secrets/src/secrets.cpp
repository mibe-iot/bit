#include <secrets.h>

extern const uint8_t secrets_json_start[] asm("_binary_secrets_json_start");
extern const uint8_t secrets_json_end[] asm("_binary_secrets_json_end");

DynamicJsonDocument doc(1024);

JsonObject Secrets::GetJsonObject()
{
    deserializeJson(doc, secrets_json_start);
    return doc.as<JsonObject>();
}

std::string Secrets::GetBLEServiceName()
{
    deserializeJson(doc, secrets_json_start);
    JsonObject obj = doc.as<JsonObject>();
    return obj[F("SERVICE_NAME")];
}

std::string Secrets::GetBLEServiceUUID()
{
    deserializeJson(doc, secrets_json_start);
    JsonObject obj = doc.as<JsonObject>();
    return obj[F("SERVICE_UUID")];
}

std::string Secrets::GetWiFiSSID()
{
    deserializeJson(doc, secrets_json_start);
    JsonObject obj = doc.as<JsonObject>();
    return obj[F("WIFI_SSID")];
}

std::string Secrets::GetWiFiPassword()
{
    deserializeJson(doc, secrets_json_start);
    JsonObject obj = doc.as<JsonObject>();
    return obj[F("WIFI_PASSWORD")];
}
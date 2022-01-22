#include <secrets.h>
#include <ArduinoJson.h>

extern const uint8_t secrets_json_start[] asm("_binary_secrets_json_start");
extern const uint8_t secrets_json_end[] asm("_binary_secrets_json_end");

DynamicJsonDocument doc(1024);

std::string Secrets::GetBLEServiceName()
{
    deserializeJson(doc, secrets_json_start);
    JsonObject obj = doc.as<JsonObject>();
    return obj[F("SERVICE_NAME")];
}
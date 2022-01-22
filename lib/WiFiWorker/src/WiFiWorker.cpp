#include <WiFiWorker.h>
#include <WiFi.h>
#include <secrets.h>

portMUX_TYPE serialMux = portMUX_INITIALIZER_UNLOCKED;

void WiFiWorker::TaskHandler(void *param)
{
    EventGroupHandle_t flags = (EventGroupHandle_t)param;

    WiFi.persistent(false);
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();

    JsonObject secrets = Secrets::GetJsonObject();

    const char *ssid = secrets[F("WIFI_SSID")];
    const char *password = secrets[F("WIFI_PASSWORD")];

    while (true)
    {
        if (WiFi.isConnected())
        {
            vTaskDelay(pdMS_TO_TICKS(100));
            continue;
        }

        xEventGroupClearBits(flags, WifiConnected);

        WiFi.begin(ssid, password);

        portENTER_CRITICAL(&serialMux);
        Serial.print("Connecting to SSID \"");
        Serial.print(ssid);
        Serial.println("\"...");
        portEXIT_CRITICAL(&serialMux);

        uint32_t start = millis();
        while ((!WiFi.isConnected()) && (millis() - start < WifiTimeout))
        {
            vTaskDelay(pdMS_TO_TICKS(500));
        }

        if (WiFi.isConnected())
        {
            xEventGroupSetBits(flags, WifiConnected);

            portENTER_CRITICAL(&serialMux);
            Serial.print("Connected to WiFi with IP ");
            Serial.println(WiFi.localIP());
            portEXIT_CRITICAL(&serialMux);
        }
        else
        {
            WiFi.disconnect();
            Serial.println("Failed to connect to WiFi!");
            vTaskDelay(pdMS_TO_TICKS(WifiTimeout));
        }
    }
}
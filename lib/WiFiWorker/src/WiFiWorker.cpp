#include <WiFiWorker.h>
#include <WiFi.h>
#include <SharedState.h>
#include <BLEWorker.h>

portMUX_TYPE serialMux = portMUX_INITIALIZER_UNLOCKED;

[[noreturn]] void WiFiWorker::Connect(void *param) {
    auto state = (SharedState *) param;

    while (true) {
        if (WiFi.isConnected()) {
            vTaskDelay(pdMS_TO_TICKS(100));
            continue;
        }

        xEventGroupClearBits(state->flags, SharedConnectivityState::WIFI_CONNECTED);

        WiFi.begin(state->configuration->wifi->GetSSID().c_str(), state->configuration->wifi->GetPassword().c_str());

        Serial.print("Connecting to SSID \"");
        Serial.print(state->configuration->wifi->GetSSID().c_str());
        Serial.println("\"...");

        uint32_t start = millis();
        while ((!WiFi.isConnected()) && (millis() - start < WifiTimeout)) {
            vTaskDelay(pdMS_TO_TICKS(500));
        }

        if (WiFi.isConnected()) {
            xEventGroupSetBits(state->flags, SharedConnectivityState::WIFI_CONNECTED);
            portENTER_CRITICAL(&serialMux);
            Serial.print("Connected to WiFi with IP ");
            Serial.println(WiFi.localIP());
            portEXIT_CRITICAL(&serialMux);
        } else {
            WiFi.disconnect();
            Serial.println("Failed to connect to WiFi!");
            vTaskDelay(pdMS_TO_TICKS(WifiTimeout));
        }
    }
}

[[noreturn]] void WiFiWorker::TaskHandler(void *param) {
    auto state = (SharedState *) param;

    WiFi.persistent(false);
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();

    EventBits_t uxBits;

    while (true) {
        uxBits = xEventGroupWaitBits(
                state->flags,
                SharedConnectivityState::SSID_RECEIVED | SharedConnectivityState::PASSWORD_RECEIVED,
                pdFALSE,
                pdTRUE,
                pdMS_TO_TICKS(20));
        if ((uxBits & (SharedConnectivityState::SSID_RECEIVED | SharedConnectivityState::PASSWORD_RECEIVED)) ==
            (SharedConnectivityState::SSID_RECEIVED | SharedConnectivityState::PASSWORD_RECEIVED)) {
            xTaskCreate(WiFiWorker::Connect, "WiFiConnector", 4096, state, tskIDLE_PRIORITY, NULL);
            vTaskDelete(NULL);
        }
        vTaskDelay(pdMS_TO_TICKS(300));
    }
}

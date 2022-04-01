#include <WiFiWorker.h>
#include <WiFi.h>
#include <SharedState.h>
#include <BLEWorker.h>

const char TAG[] = "Wi-Fi";

[[noreturn]] void WiFiWorker::Connect(void *param) {
    auto state = (SharedState *) param;

    while (true) {
        if (WiFi.isConnected()) {
            vTaskDelay(pdMS_TO_TICKS(100));
            continue;
        }

        xEventGroupClearBits(state->flags, SharedConnectivityState::WIFI_CONNECTED);
        WiFi.begin(state->configuration->wifi->GetSSID().c_str(), state->configuration->wifi->GetPassword().c_str());

        uint32_t start = millis();
        while ((!WiFi.isConnected()) && (millis() - start < WifiTimeout)) {
            vTaskDelay(pdMS_TO_TICKS(500));
        }

        if (WiFi.isConnected()) {
            xEventGroupSetBits(state->flags, SharedConnectivityState::WIFI_CONNECTED);
        } else {
            WiFi.disconnect();
            ESP_LOGE(TAG, "failed to connect to Wi-Fi with SSID \"%s\"", state->configuration->wifi->GetSSID().c_str());
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
                pdMS_TO_TICKS(10));
        if ((uxBits & (SharedConnectivityState::SSID_RECEIVED | SharedConnectivityState::PASSWORD_RECEIVED)) ==
            (SharedConnectivityState::SSID_RECEIVED | SharedConnectivityState::PASSWORD_RECEIVED)) {
            xTaskCreate(WiFiWorker::Connect, "WiFiConnector", 4096, state, 4 | portPRIVILEGE_BIT, NULL);
            vTaskDelete(NULL);
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

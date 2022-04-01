#include <MQTTWorker.h>
#include <WiFi.h>
#include <SharedState.h>
#include <ESPmDNS.h>

const char TAG[] = "MQTT";

void MQTTWorker::TaskHandler(void *param) {
    auto state = (SharedState *) param;

    WiFiClient client;
    state->mqtt->setClient(client);
    state->mqtt->setCallback([](char *topic, uint8_t *payload, unsigned int len) {
        Serial.print("MQTT topic \"");
        Serial.print(topic);
        Serial.print("\" with value \"");
        for (uint16_t i = 0; i < len; ++i)
            Serial.print((char) payload[i]);
        Serial.print("\" (");
        Serial.print(len);
        Serial.println(" byte(s)) received");
    });

    while (true) {
        EventBits_t uxBits = xEventGroupWaitBits(state->flags, SharedConnectivityState::WIFI_CONNECTED, pdFALSE, pdTRUE,
                                                 pdMS_TO_TICKS(200));
        if ((uxBits & SharedConnectivityState::WIFI_CONNECTED) == SharedConnectivityState::WIFI_CONNECTED) {
            if (!state->mqtt->connected()) {
                mdns_init();
                auto ip = MDNS.queryHost(MQTT_SERVER, 5000);
                state->mqtt->setServer(MQTT_SERVER, MQTT_PORT);

                xEventGroupClearBits(state->flags, SharedConnectivityState::MQTT_CONNECT);
                if (state->mqtt->connect(MQTT_CLIENT)) {
                    if (!state->mqtt->subscribe("#")) {
                        ESP_LOGE(TAG, "error when trying to subscribe to all topics");
                    }

                    DynamicJsonDocument document(1024);
                    document[F("deviceId")] = state->configuration->mqtt->GetDeviceIdentifier();
                    document[F("deviceClass")] = "fervent";
                    document[F("actions")][0][F("name")] = "atmosphere";
                    document[F("reportTypes")][0] = "data";

                    char buf[1024];
                    serializeJson(document, buf, 1024);

                    if (!state->mqtt->publish("/mibe/actions", buf)) {
                        ESP_LOGE(TAG, "cannot sent actions to broker");
                    }

                    xEventGroupSetBits(state->flags, SharedConnectivityState::MQTT_CONNECT);
                } else {
                    ESP_LOGE(TAG, "failed to connect to MQTT");
                    vTaskDelay(pdMS_TO_TICKS(5000));
                }
            } else state->mqtt->loop();
        }
        vTaskDelay(pdMS_TO_TICKS(300));
    }
}
#include <MQTTWorker.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <SharedState.h>
#include <ESPmDNS.h>

void MQTTWorker::TaskHandler(void *param) {
    auto state = (SharedState *) param;

    WiFiClient client;
    PubSubClient mqtt(client);
    uint32_t lastPublish = 0;

    mqtt.setCallback([](char *topic, uint8_t *payload, unsigned int len) {
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
            if (!mqtt.connected()) {
                mdns_init();

                auto ip = MDNS.queryHost("thinker", 5000);

                mqtt.setServer(ip.toString().c_str(), MQTT_PORT);

                xEventGroupClearBits(state->flags, SharedConnectivityState::MQTT_CONNECT);
                if (mqtt.connect(MQTT_CLIENT)) {
                    xEventGroupSetBits(state->flags, SharedConnectivityState::MQTT_CONNECT);
                    if (!mqtt.subscribe("#")) {
                        Serial.println("Error subscribing to all topics!");
                    }

                    DynamicJsonDocument document(1024);
                    document[F("deviceId")] = state->configuration->wifi->GetIdentifier();
                    document[F("deviceClass")] = "fervent";
                    document[F("actions")][0][F("name")] = "atmosphere";
                    document[F("reportTypes")][0] = "data";

                    char buf[1024];
                    serializeJson(document, buf, 1024);
                    ;
                    if (!mqtt.publish("/mibe/actions", buf)) {
                        ESP_LOGE("MQTT", "cannot sent actions to broker");
                    }
                } else {
                    Serial.println("Failed to connect to MQTT!");
                    Serial.println("Wifi ip" + client.localIP().toString());
                }
            } else {
                uint32_t time;
                time = millis();
                if ((!lastPublish) || (time - lastPublish >= MQTT_TIMEOUT)) {
                    char data[11];
                    ultoa(time, data, 10);

                    DynamicJsonDocument document(1024);
                    document[F("reportType")] = "data";
                    document[F("reportData")][F("uptime")] = std::string(data);

                    char buf[1024];
                    serializeJson(document, buf, 1024);

                    if (!mqtt.publish(("/mibe/reports/" + state->configuration->wifi->GetIdentifier()).c_str(), buf)) {
                        Serial.println("Error publishing uptime topic!");
                    }
                    lastPublish = time;
                }
                mqtt.loop();
            }
        }
        vTaskDelay(pdMS_TO_TICKS(300));
    }
}
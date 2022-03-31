#include <MQTTWorker.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <secrets.h>
#include <ESPmDNS.h>

void MQTTWorker::TaskHandler(void *param) {
    EventGroupHandle_t flags = (EventGroupHandle_t) param;

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
        EventBits_t uxBits = xEventGroupWaitBits(flags, SharedConnectivityState::WIFI_CONNECTED, pdFALSE, pdTRUE,
                                                 pdMS_TO_TICKS(200));
        if ((uxBits & SharedConnectivityState::WIFI_CONNECTED) == SharedConnectivityState::WIFI_CONNECTED) {
            if (!mqtt.connected()) {
                mdns_init();

                auto ip = MDNS.queryHost("thinker", 5000);

                mqtt.setServer(ip.toString().c_str(), MQTT_PORT);

                xEventGroupClearBits(flags, SharedConnectivityState::MQTT_CONNECT);
                if (mqtt.connect(MQTT_CLIENT)) {
                    xEventGroupSetBits(flags, SharedConnectivityState::MQTT_CONNECT);
                    if (!mqtt.subscribe("#")) {
                        Serial.println("Error subscribing to all topics!");
                    }

                    char buf[4096];
                    sprintf(buf, "{"
                                 "  \"deviceName\": \"6240809d83c9875a52244a7a\","
                                 "  \"actions\": ["
                                 "      {"
                                 "          \"name\": \"uptime\""
                                 "      },"
                                 "      {"
                                 "          \"name\": \"humidity\""
                                 "      }"
                                 "  ]"
                                 "}");
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
                    char str[4096];
                    sprintf(str, "{"
                                 "  \"reportData\": {"
                                 "      \"kekw\": \"lol\","
                                 "      \"uptime\": \"%s\""
                                 "  }"
                                 "}", data);
                    if (!mqtt.publish("/mibe/reports/6240809d83c9875a52244a7a", str)) {
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
#include <AtmosphereWorker.h>
#include <SharedState.h>
#include <WiFiWorker.h>
#include <BLEWorker.h>
#include <MQTTWorker.h>

#define LED 5

[[noreturn]] void NotifierWorker(void *param) {
    auto flags = (EventGroupHandle_t) param;
    EventBits_t uxBits;

    while (true) {
        digitalWrite(LED, LOW);
        uxBits = xEventGroupWaitBits(flags, SharedConnectivityState::WIFI_CONNECTED, pdFALSE, pdTRUE,
                                     pdMS_TO_TICKS(100));
        if ((uxBits & SharedConnectivityState::WIFI_CONNECTED) == SharedConnectivityState::WIFI_CONNECTED) {
            digitalWrite(LED, HIGH);
        }
        vTaskDelay(pdMS_TO_TICKS(300));
    }
}

void setup() {
    EventGroupHandle_t flags = xEventGroupCreate();
    auto actions = new ActionsService();
    auto configuration = new SharedConfiguration();
    auto mqtt = new PubSubClient();
    auto state = new SharedState{actions, flags, mqtt, configuration};
    state->CheckConfiguration();

    actions->registerAction("/mibe/reports/fnfj", [](std::string buf){
        ESP_LOGE("MAIN", "OK");
        Serial.printf("MQTT topic received message: %s", buf.c_str());
    });

    pinMode(LED, OUTPUT);
    digitalWrite(LED, LOW);
    Serial.begin(115200);

    xTaskCreate(BLEWorker::TaskHandler, "BLEWorker::TaskHandler", 8192, state, 1 | portPRIVILEGE_BIT, nullptr);
    xTaskCreate(WiFiWorker::TaskHandler, "WiFiWorker::TaskHandler", 8192, state, 4 | portPRIVILEGE_BIT, nullptr);
    xTaskCreate(MQTTWorker::TaskHandler, "MQTTWorker::TaskHandler", 8192, state, 1 | portPRIVILEGE_BIT, nullptr);
    xTaskCreate(AtmosphereWorker::TaskHandler, "AtmosphereWorker::TaskHandler", 8192, state, 1 | portPRIVILEGE_BIT, nullptr);
    xTaskCreate(NotifierWorker, "Notifier", 1024, flags, 1 | portPRIVILEGE_BIT, nullptr);
}

void loop() {
    vTaskDelete(nullptr);
}
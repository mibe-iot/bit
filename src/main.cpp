#include <Arduino.h>
#include <SharedState.h>
#include <WiFiWorker.h>
#include <BLEWorker.h>
#include <MQTTWorker.h>

#define LED 2

[[noreturn]] void NotifierWorker(void *param) {
    auto flags = (EventGroupHandle_t) param;
    EventBits_t uxBits;

    while (true) {
        digitalWrite(LED, LOW);
        uxBits = xEventGroupWaitBits(flags, SharedConnectivityState::WIFI_CONNECTED, pdFALSE, pdTRUE,
                                     pdMS_TO_TICKS(20));
        if ((uxBits & SharedConnectivityState::WIFI_CONNECTED) == SharedConnectivityState::WIFI_CONNECTED) {
            digitalWrite(LED, HIGH);
        }
        vTaskDelay(pdMS_TO_TICKS(300));
    }
}

void setup() {
    EventGroupHandle_t flags = xEventGroupCreate();
    auto configuration = new SharedConfiguration();

    auto state = new SharedState{ flags, configuration };
    state->CheckConfiguration();

    pinMode(LED, OUTPUT);
    digitalWrite(LED, LOW);
    Serial.begin(115200);

    xTaskCreate(BLEWorker::TaskHandler, "BLEWorker::TaskHandler", 8192, state, 1, nullptr);
    xTaskCreate(WiFiWorker::TaskHandler, "WiFiWorker::TaskHandler", 8192, state, 2, nullptr);
    xTaskCreate(MQTTWorker::TaskHandler, "MQTTWorker::TaskHandler", 8192, state, 1, nullptr);
    xTaskCreate(NotifierWorker, "Notifier", 1024, flags, 10, nullptr);

    Serial.println("Waiting a client connection to notify...");
}

void loop() {
    vTaskDelete(nullptr);
}
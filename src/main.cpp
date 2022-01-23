#include <Arduino.h>
#include <secrets.h>
#include <WiFiWorker.h>
#include <BLEWorker.h>

void setup() {
  EventGroupHandle_t flags = xEventGroupCreate();

  Serial.begin(115200);

  xTaskCreate(BLEWorker::TaskHandler, "BLEWorker::TaskHandler", 8192, flags, 1, nullptr);
  xTaskCreate(WiFiWorker::TaskHandler, "WiFiWorker::TaskHandler", 8192, flags, 1, nullptr);

  Serial.println("Waiting a client connection to notify...");
}

void loop() {
  vTaskDelete(nullptr);
}
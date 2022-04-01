#include <AtmosphereWorker.h>
#include <SharedState.h>
#include <ArduinoJson.h>
#include <DHTesp.h>

const char TAG[] = "DHT";

void AtmosphereWorker::TaskHandler(void *param) {
    auto state = (SharedState *) param;
    DHTesp dht;
    dht.setup(15, DHTesp::DHT11);

    while (true) {
        auto result = xEventGroupWaitBits(state->flags, SharedConnectivityState::MQTT_CONNECT, pdFALSE, pdTRUE,
                                          pdMS_TO_TICKS(100));
        if (!(result & SharedConnectivityState::MQTT_CONNECT)) {
            vTaskDelay(pdMS_TO_TICKS(300));
            continue;
        }

        DynamicJsonDocument document(1024);
        document[F("reportType")] = "atmosphere";
        document[F("reportData")]["temperature"] = dht.getTemperature();
        document[F("reportData")]["humidity"] = dht.getHumidity();

        char buf[1024];
        serializeJson(document, buf, 1024);

        auto endpoint = "/mibe/reports/" + state->configuration->mqtt->GetDeviceIdentifier();
        if (!state->mqtt->publish(endpoint.c_str(), buf)) {
            ESP_LOGE(TAG, "cannot to report atmosphere data");
        }

        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

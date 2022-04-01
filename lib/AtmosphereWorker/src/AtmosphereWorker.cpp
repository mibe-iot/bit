#include <AtmosphereWorker.h>
#include <SharedState.h>
#include <DHTesp.h>

void AtmosphereWorker::TaskHandler(void *param) {
    auto state = (SharedState *) param;
    DHTesp dht;
    dht.setup(15, DHTesp::DHT11);

    while (true) {
        auto result = xEventGroupWaitBits(state->flags, SharedConnectivityState::WIFI_CONNECTED, pdFALSE, pdTRUE,
                                          pdMS_TO_TICKS(100));
        if (!(result & SharedConnectivityState::WIFI_CONNECTED)) {
            vTaskDelay(pdMS_TO_TICKS(300));
            continue;
        }

        float temperature = dht.getTemperature();
        float humidity = dht.getHumidity();
        Serial.printf("temperature: %f, humidity: %f\n", temperature, humidity);
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

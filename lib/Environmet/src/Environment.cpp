#include <Environment.h>

void Environment::begin(DHT_Unified &dht) {
  dht.begin();
  dht.temperature().getSensor(&sensor);
  dht.humidity().getSensor(&sensor);
  minDelay = sensor.min_delay / 1000;
  lastReadings = millis();
}

Environment::Metric Environment::getMetric(DHT_Unified &dht) {
  if (millis() - lastReadings < minDelay) {
    return {previosTemperature, previosHumidity};
  }

  dht.temperature().getEvent(&event);
  previosTemperature = event.temperature;

  dht.humidity().getEvent(&event);
  previosHumidity = event.relative_humidity;

  lastReadings = millis();

  return {previosTemperature, previosHumidity};
}
#pragma once

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

class Environment {
public:
  struct Metric {
    float temperature;
    float humidity;
  };

public:
  void begin(DHT_Unified &dht);
  Metric getMetric(DHT_Unified &dht);

private:
  DHT_Unified *dht;
  sensor_t sensor;
  sensors_event_t event;

  float previosTemperature = 0;
  float previosHumidity = 0;

  unsigned long minDelay;
  unsigned long lastReadings;
};
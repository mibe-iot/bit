
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#include <Environment.h>
#include <Pairing.h>
#include <buttons.h>

#define DHTPIN 13
#define DHTTYPE DHT11

DHT_Unified dht(DHTPIN, DHTTYPE);

Pairing pairing("unattached", "12345678");
Environment environment;

Button backButton(BACK_BUTTON_PIN, &backButtonHandler);
Button forwardButton(FORWARD_BUTTON_PIN, &forwardButtonHandler);
Button actionButton(ACTION_BUTTON_PIN, []() {
  auto metric = environment.getMetric(dht);
  Serial.printf("%f; %f;\n", metric.temperature, metric.humidity);
});

void setup() {
  Serial.begin(9600);

  environment.begin(dht);

  backButton.setup();
  forwardButton.setup();
  actionButton.setup();
  pairing.begin([]() { Serial.println("Connected"); });
}

void loop() {
  pairing.handleConnection();
  backButton.handlePushEvent();
  actionButton.handlePushEvent();
  forwardButton.handlePushEvent();
}

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#include <WebSocketsServer.h>

#include <ArduinoJson.h>

#include <Environment.h>
#include <Pairing.h>
#include <buttons.h>

#define DHTPIN 13
#define DHTTYPE DHT11

DHT_Unified dht(DHTPIN, DHTTYPE);

Pairing pairing("unattached2", "12345678");
WebSocketsServer event(81);

Environment environment;

Button backButton(BACK_BUTTON_PIN, []() {
  DynamicJsonDocument doc(128);

  doc["type"] = "button";
  doc["value"] = "back";

  std::string buffer;
  serializeJson(doc, buffer);
  event.broadcastTXT(buffer.c_str(), (size_t)buffer.size());
});

Button forwardButton(FORWARD_BUTTON_PIN, []() {
  DynamicJsonDocument doc(128);

  doc["type"] = "button";
  doc["value"] = "forward";

  std::string buffer;
  serializeJson(doc, buffer);
  event.broadcastTXT(buffer.c_str(), (size_t)buffer.size());
});

Button actionButton(ACTION_BUTTON_PIN, []() {
  DynamicJsonDocument doc(128);

  doc["type"] = "button";
  doc["value"] = "action";

  std::string buffer;
  serializeJson(doc, buffer);
  event.broadcastTXT(buffer.c_str(), (size_t)buffer.size());
});

void handleWSMessage(uint8_t *payload, size_t length) {
  DynamicJsonDocument doc(128);
  deserializeJson(doc, payload);

  std::string type = doc["type"];
  Serial.println(type.c_str());
  if (type == "metric") {
    doc.clear();
    doc["type"] = "metric";

    auto metric = environment.getMetric(dht);
    doc["temperature"] = metric.temperature;
    doc["humidity"] = metric.humidity;
  }
}

void handleWSEvent(uint8_t num, WStype_t type, uint8_t *payload,
                   size_t length) {

  switch (type) {
  case WStype_DISCONNECTED:
    Serial.printf("[%u] Disconnected!\n", num);
    break;
  case WStype_CONNECTED: {
    IPAddress ip = event.remoteIP(num);
    Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0],
                  ip[1], ip[2], ip[3], payload);

    // send message to client
    event.sendTXT(num, "Connected");
  } break;
  case WStype_TEXT: {
    handleWSMessage(payload, length);
    break;
  }
  }
}

void setup() {
  Serial.begin(9600);

  environment.begin(dht);

  backButton.setup();
  forwardButton.setup();
  actionButton.setup();

  event.begin();
  event.onEvent(handleWSEvent);

  pairing.begin([]() {
    digitalWrite(2, 1);

    Serial.println("Connected");
    Serial.println(WiFi.localIP());
  });
}

void loop() {
  event.loop();

  pairing.handleConnection();
  backButton.handlePushEvent();
  actionButton.handlePushEvent();
  forwardButton.handlePushEvent();
}

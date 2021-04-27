#include <Arduino.h>
#include <DHT.h>
#include <DHT_U.h>

#include <Pairing.h>
#include <buttons.h>

#define DHTPIN A15
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);

  Button homeButton = {HOME_BUTTON_PIN, &homeButtonHandler};
  Button backButton = {BACK_BUTTON_PIN, &backButtonHandler};
  Button forwardButton = {FORWARD_BUTTON_PIN, &forwardButtonHandler};
  Button actionButton = {ACTION_BUTTON_PIN, &actionButtonHandler};

  ButtonController::registerButton(homeButton);
  ButtonController::registerButton(backButton);
  ButtonController::registerButton(forwardButton);
  ButtonController::registerButton(actionButton);

  Pairing pairing("kekw", "12345678");
  pairing.onPair([](std::string ssid, std::string password) {
    Serial.println("ssid:");
    Serial.println(ssid.c_str());

    Serial.println("password:");
    Serial.println(password.c_str());
  });
}

void loop() {}
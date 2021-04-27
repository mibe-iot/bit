#include <Arduino.h>

#include <Button.h>

#define HOME_BUTTON_PIN 17
#define ACTION_BUTTON_PIN 18
#define BACK_BUTTON_PIN 5
#define FORWARD_BUTTON_PIN 21

void IRAM_ATTR forwardButtonHandler() {
  ButtonController::debounce(
      FORWARD_BUTTON_PIN, []() { Serial.println("Pressed forward button"); });
}

void IRAM_ATTR backButtonHandler() {
  ButtonController::debounce(BACK_BUTTON_PIN,
                             []() { Serial.println("Pressed back button"); });
}

void IRAM_ATTR homeButtonHandler() {
  ButtonController::debounce(HOME_BUTTON_PIN,
                             []() { Serial.println("Pressed home button"); });
}

void IRAM_ATTR actionButtonHandler() {
  ButtonController::debounce(ACTION_BUTTON_PIN,
                             []() { Serial.println("Pressed action button"); });
}

void setup() {
  Serial.begin(9600);

  Button homeButton = {HOME_BUTTON_PIN, &homeButtonHandler};
  Button backButton = {BACK_BUTTON_PIN, &backButtonHandler};
  Button forwardButton = {FORWARD_BUTTON_PIN, &forwardButtonHandler};
  Button actionButton = {ACTION_BUTTON_PIN, &forwardButtonHandler};

  ButtonController::registerButton(homeButton);
  ButtonController::registerButton(backButton);
  ButtonController::registerButton(forwardButton);
  ButtonController::registerButton(actionButton);
}

void loop() {}
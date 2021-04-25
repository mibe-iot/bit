#include <Arduino.h>

#include <Button.h>

void IRAM_ATTR homeButtonHandler() { Serial.println("Pressed home button"); }
void IRAM_ATTR backButtonHandler() { Serial.println("Pressed back button"); }
void IRAM_ATTR forwardButtonHandler() {
  ButtonController::debounce(
      21, []() { Serial.println("Pressed forward button"); });
}

void setup() {
  Serial.begin(9600);

  Button homeButton = {18, &homeButtonHandler};
  Button backButton = {5, &backButtonHandler};
  Button forwardButton = {21, &forwardButtonHandler};

  ButtonController::registerButton(homeButton);
  ButtonController::registerButton(backButton);
  ButtonController::registerButton(forwardButton);
}

void loop() {}
#include <Arduino.h>

#include <Button.h>

#define ACTION_BUTTON_PIN 15
#define BACK_BUTTON_PIN 5
#define FORWARD_BUTTON_PIN 14

void IRAM_ATTR forwardButtonHandler() {
  Serial.println("Pressed forward button");
}

void IRAM_ATTR backButtonHandler() { Serial.println("Pressed back button"); }

void IRAM_ATTR actionButtonHandler() {
  Serial.println("Pressed action button");
}
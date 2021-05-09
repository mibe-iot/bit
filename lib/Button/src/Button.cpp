#include <Arduino.h>

#include <map>

#include <Button.h>

Button::Button(int pin, void (*handler)()) {
  this->pin = pin;
  this->handler = handler;
  for (int i = 0; i < HISTORY_LENGTH; i++)
    history[i] = HIGH;
}

void Button::setup() { digitalWrite(pin, HIGH); }

void Button::addToHistory(int value) {
  for (int i = 0; i < HISTORY_LENGTH - 1; i++) {
    history[i] = history[i + 1];
  }
  history[HISTORY_LENGTH - 1] = value;
}

bool Button::isPushed() {
  int pinStatus = digitalRead(pin);
  addToHistory(pinStatus);

  for (int i = HISTORY_LENGTH - 1, highThreshold = 0, lowThreshold = 0; i >= 0;
       i--) {
    if (history[i] == HIGH) {
      highThreshold++;
      if (highThreshold == HISTORY_THRESHOLD) {
        return false;
      }
    }

    if (history[i] == LOW) {
      highThreshold = 0;
      lowThreshold++;
      history[HISTORY_LENGTH - 1] = HISTORY_PUSHED;
      if (history[HISTORY_LENGTH - 2] == HISTORY_PUSHED) {
        return false;
      } else {
        return true;
      }
    }
  }

  return false;
}

void Button::handlePushEvent() {
  if (isPushed())
    this->handler();
}
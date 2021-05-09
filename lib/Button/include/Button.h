#pragma once

#include <functional>

#include "driver/gpio.h"

#define HISTORY_LENGTH 100
#define HISTORY_THRESHOLD 30
#define HISTORY_PUSHED 2

class Button {
public:
  Button(int pin, void (*handler)());

public:
  void setup();
  bool isPushed();
  void handlePushEvent();

private:
  void addToHistory(int value);

private:
  int pin;
  void (*handler)();
  char history[HISTORY_LENGTH];
};
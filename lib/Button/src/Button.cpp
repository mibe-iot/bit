#include <Arduino.h>

#include <map>

#include <Button.h>

static std::map<int, time_t> buttons;

portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;

void ButtonController::registerButton(Button button) {
  pinMode(button.PIN, INPUT_PULLUP);
  auto handle = digitalPinToInterrupt(button.PIN);
  attachInterrupt(handle, button.handler, FALLING);
  buttons[button.PIN] = time(0);
}

void ButtonController::debounce(int PIN, std::function<void(void)> handler) {
  portENTER_CRITICAL_ISR(&mux);
  if (buttons.find(PIN) != buttons.end())
    if (difftime(time(0), buttons[PIN]) > 0.01) {
      handler();
      buttons[PIN] = time(0);
    }
  portEXIT_CRITICAL_ISR(&mux);
}
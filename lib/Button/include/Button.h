#include <functional>

#include "driver/gpio.h"

struct Button {
  int PIN;
  void (*handler)();
};

class ButtonController {
public:
  static void registerButton(Button button);
  static void debounce(int PIN, std::function<void(void)> handler);
};
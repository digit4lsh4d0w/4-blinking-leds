#include "board.h"
#include "button_handler.h"
#include "effects_engine.h"
#include "pwm_engine.h"
#include <stdio.h>

int main(void) {
  printf("Starting!\n");

  pwm_engine_init();
  logic_engine_init();
  button_handler_init(BTN0_PIN);

  return 0;
}

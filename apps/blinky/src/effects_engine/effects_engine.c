#include "effects_engine.h"
#include "effects_internal.h"
#include "led.h"
#include "pwm_engine.h"
#include "thread.h"
#include "thread_config.h"
#include "ztimer.h"
#include <stdint.h>

static constexpr uint32_t LOGIC_PERIOD_MS = 20;
// Работа начинается в режиме №1.
static effect_mode_t current_mode = MODE_1;

// Контракт
typedef struct {
  void (*update)(uint32_t time_ms);
  void (*on_short_press)();
} effect_strategy_t;

// v-table
static const effect_strategy_t EFFECTS[MODES_COUNT] = {
    [MODE_1] =
        {
            .update = effect_mode_1_update,
            .on_short_press = effect_mode_1_short_press,
        },
    [MODE_2] =
        {
            .update = effect_mode_2_update,
            .on_short_press = effect_mode_2_short_press,
        },
};

void logic_engine_on_short_press() {
  if (EFFECTS[current_mode].on_short_press != nullptr) {
    EFFECTS[current_mode].on_short_press();
  }
}

void logic_engine_on_long_press() {
  current_mode = (current_mode + 1) % MODES_COUNT;

  for (int i = 0; i < COLORS_COUNT; i++) {
    pwm_set_brightness(i, PWM_MIN_BRIGHTNESS);
  }
}

static void *logic_thread([[maybe_unused]] void *arg) {
  uint32_t last_wakeup = ztimer_now(ZTIMER_MSEC);

  while (true) {
    uint32_t current_time = ztimer_now(ZTIMER_MSEC);

    if (EFFECTS[current_mode].update != nullptr) {
      EFFECTS[current_mode].update(current_time);
    }

    ztimer_periodic_wakeup(ZTIMER_MSEC, &last_wakeup, LOGIC_PERIOD_MS);
  }

  return nullptr;
}

void logic_engine_next_mode() {
  current_mode = (current_mode + 1) % MODES_COUNT;
}

void logic_engine_init() {
  static char logic_thread_stack[THREAD_STACKSIZE_MAIN];
  thread_create(logic_thread_stack, sizeof(logic_thread_stack),
                THREAD_PRIORITY_MAIN - 1, 0, logic_thread, nullptr, "logic");
}

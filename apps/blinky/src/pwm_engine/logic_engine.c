#include "logic_engine.h"
#include "led.h"
#include "pwm_engine.h"
#include "thread.h"
#include "thread_config.h"
#include "ztimer.h"
#include <stdint.h>

static constexpr uint32_t LOGIC_PERIOD_MS = 20;
// Работа начинается в режиме №1.
static blinking_mode_t current_mode = MODE_1;
// Флаг короткого нажатия.
static _Atomic bool btn_short_pressed = false;

typedef struct {
  void (*on_enter)();
  void (*update)(uint32_t time_ms, bool btn_pressed);
} effect_strategy_t;

static void effect_mode_1_enter() {}

static void effect_mode_1_update(uint32_t time_ms, bool btn_pressed) {}

static void effect_mode_2_enter() {}

static void effect_mode_2_update(uint32_t time_ms, bool btn_pressed) {}

static const effect_strategy_t EFFECTS[MODES_COUNT] = {
    [MODE_1] =
        {
            .on_enter = effect_mode_1_enter,
            .update = effect_mode_1_update,
        },
    [MODE_2] =
        {
            .on_enter = effect_mode_2_enter,
            .update = effect_mode_2_update,
        },
};

static void *logic_thread([[maybe_unused]] void *arg) {
  uint32_t last_wakeup = ztimer_now(ZTIMER_MSEC);

  if (EFFECTS[current_mode].on_enter != nullptr) {
    EFFECTS[current_mode].on_enter();
  }

  while (true) {
    uint32_t current_time = ztimer_now(ZTIMER_MSEC);

    if (EFFECTS[current_mode].update != nullptr) {
      EFFECTS[current_mode].update(current_time);
    }

    ztimer_periodic_wakeup(ZTIMER_MSEC, &last_wakeup, LOGIC_PERIOD_MS);
  }
}

void logic_engine_next_mode() {
  current_mode = (current_mode + 1) % MODES_COUNT;
}

void logic_engine_init() {
  static char logic_thread_stack[THREAD_STACKSIZE_MAIN];
  thread_create(logic_thread_stack, sizeof(logic_thread_stack),
                THREAD_PRIORITY_MAIN - 1, 0, logic_thread, nullptr, "logic");
}

#include "effects_engine.h"
#include "effects_internal.h"
#include "led.h"
#include "pwm_engine.h"

#include "thread.h"
#include "thread_config.h"
#include "ztimer.h"

#include <stdatomic.h>
#include <stdint.h>

// Контракт типа, который обозначает конкретый режим мерцания светодиодами.
typedef struct {
  void (*update)(uint32_t time_ms);
  void (*on_short_press)();
} effect_strategy_t;

// Период активации функции обновления состояния эффекта
static constexpr uint32_t EFFECT_PERIOD_MS = 20;

// Стек под поток управления эффектами
static char effect_thread_stack[THREAD_STACKSIZE_MAIN];

// Работа начинается в режиме №1.
static _Atomic effect_mode_t current_mode = EFFECT_MODE_1;

// V-Table: Таблица диспетчеризации эффектов.
static const effect_strategy_t EFFECTS[EFFECT_MODES_COUNT] = {
    [EFFECT_MODE_1] =
        {
            .update = effect_mode_1_update,
            .on_short_press = effect_mode_1_short_press,
        },
    [EFFECT_MODE_2] =
        {
            .update = effect_mode_2_update,
            .on_short_press = effect_mode_2_short_press,
        },
};

static void *effects_thread([[maybe_unused]] void *arg) {
  uint32_t last_wakeup = ztimer_now(ZTIMER_MSEC);

  while (true) {
    uint32_t current_time = ztimer_now(ZTIMER_MSEC);
    effect_mode_t mode = atomic_load(&current_mode);

    if (EFFECTS[mode].update != nullptr) {
      EFFECTS[mode].update(current_time);
    }

    ztimer_periodic_wakeup(ZTIMER_MSEC, &last_wakeup, EFFECT_PERIOD_MS);
  }

  return nullptr;
}

void effects_engine_on_short_press() {
  effect_mode_t mode = atomic_load(&current_mode);
  if (EFFECTS[mode].on_short_press != nullptr) {
    EFFECTS[mode].on_short_press();
  }
}

void effects_engine_on_long_press() {
  for (uint8_t i = 0; i < COLORS_COUNT; i++) {
    pwm_set_brightness(i, PWM_MIN_BRIGHTNESS);
  }

  effect_mode_t mode = atomic_load(&current_mode);
  atomic_store(&current_mode, (mode + 1) % EFFECT_MODES_COUNT);
}

void effects_engine_init() {
  thread_create(effect_thread_stack, sizeof(effect_thread_stack),
                THREAD_PRIORITY_MAIN - 1, 0, effects_thread, nullptr, "logic");
}

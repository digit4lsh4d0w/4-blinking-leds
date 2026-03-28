#include "effects_internal.h"
#include "led.h"
#include "pwm_engine.h"
#include <stdatomic.h>
#include <stdint.h>

static constexpr uint32_t LED1_PERIOD_MS = 1000;
static constexpr uint32_t LED2_PERIOD_MS = 500;
static constexpr uint32_t LED3_PERIOD_MS = 2000;
static constexpr uint32_t LED4_PERIOD_MS = 1000;

static _Atomic uint8_t mode_shift = 0;

// Функция генерации эффекта "дыхание".
static inline uint8_t calc_triangle_wave(uint32_t time_ms, uint32_t period,
                                         uint32_t phase_shift_ms) {
  uint32_t half_period = period / 2;
  // Текущее состояние внутри периода волны.
  uint32_t t = (time_ms + phase_shift_ms) % period;

  if (t < half_period) {
    return (t * PWM_MAX_BRIGHTNESS) / half_period;
  } else {
    return ((period - t) * PWM_MAX_BRIGHTNESS) / half_period;
  }
}

void effect_mode_1_short_press() {
  uint8_t shift = atomic_load(&mode_shift);
  atomic_store(&mode_shift, (shift + 1) % LED_COLOR_COUNT);
}

void effect_mode_1_update(uint32_t time_ms) {
  uint8_t func_brightness[LED_COLOR_COUNT];

  uint32_t led1_half_period = LED1_PERIOD_MS / 2;
  func_brightness[0] = (time_ms % LED1_PERIOD_MS < led1_half_period)
                           ? PWM_MAX_BRIGHTNESS
                           : PWM_MIN_BRIGHTNESS;

  uint32_t led2_half_period = LED2_PERIOD_MS / 2;
  func_brightness[1] = (time_ms % LED2_PERIOD_MS < led2_half_period)
                           ? PWM_MAX_BRIGHTNESS
                           : PWM_MIN_BRIGHTNESS;

  func_brightness[2] = calc_triangle_wave(time_ms, LED3_PERIOD_MS, 0);

  func_brightness[3] =
      calc_triangle_wave(time_ms, LED4_PERIOD_MS, LED4_PERIOD_MS / 2);

  uint8_t shift = atomic_load(&mode_shift);
  for (uint8_t i = 0; i < LED_COLOR_COUNT; i++) {
    int physical_led = (i + shift) % LED_COLOR_COUNT;
    pwm_set_brightness(physical_led, func_brightness[i]);
  }
}

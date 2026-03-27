#include "effects_internal.h"
#include "led.h"
#include "pwm_engine.h"
#include <stdatomic.h>
#include <stdint.h>

static constexpr uint32_t LED1_PERIOD_MS = 1000;
static constexpr uint32_t LED1_HALF_PERIOD_MS = LED1_PERIOD_MS / 2;
static constexpr uint32_t LED2_PERIOD_MS = 500;
static constexpr uint32_t LED2_HALF_PERIOD_MS = LED2_PERIOD_MS / 2;
static constexpr uint32_t LED3_PERIOD_MS = 2000;
static constexpr uint32_t LED3_HALF_PERIOD_MS = LED3_PERIOD_MS / 2;
static constexpr uint32_t LED4_PERIOD_MS = 1000;
static constexpr uint32_t LED4_HALF_PERIOD_MS = LED4_PERIOD_MS / 2;

static _Atomic uint8_t mode_shift = 0;

void effect_mode_1_short_press() {
  uint8_t shift = atomic_load(&mode_shift);
  atomic_store(&mode_shift, (shift + 1) % COLORS_COUNT);
}

void effect_mode_1_update(uint32_t time_ms) {
  uint8_t func_brightness[COLORS_COUNT];

  func_brightness[0] = (time_ms % LED1_PERIOD_MS < LED1_HALF_PERIOD_MS)
                           ? PWM_MAX_BRIGHTNESS
                           : PWM_MIN_BRIGHTNESS;
  func_brightness[1] = (time_ms % LED2_PERIOD_MS < LED2_HALF_PERIOD_MS)
                           ? PWM_MAX_BRIGHTNESS
                           : PWM_MIN_BRIGHTNESS;

  uint32_t t2 = time_ms % LED3_PERIOD_MS;
  if (t2 < LED3_HALF_PERIOD_MS) {
    func_brightness[2] = t2 * PWM_MAX_BRIGHTNESS / LED3_HALF_PERIOD_MS;
  } else {
    func_brightness[2] =
        (LED3_PERIOD_MS - t2) * PWM_MAX_BRIGHTNESS / LED3_HALF_PERIOD_MS;
  }

  uint32_t t3 = time_ms % LED4_PERIOD_MS;
  if (t3 < LED4_HALF_PERIOD_MS) {
    func_brightness[3] =
        (LED4_HALF_PERIOD_MS - t3) * PWM_MAX_BRIGHTNESS / LED4_HALF_PERIOD_MS;
  } else {
    func_brightness[3] =
        (t3 - LED4_HALF_PERIOD_MS) * PWM_MAX_BRIGHTNESS / LED4_HALF_PERIOD_MS;
  }

  uint8_t shift = atomic_load(&mode_shift);
  for (uint8_t i = 0; i < COLORS_COUNT; i++) {
    int physical_led = (i + shift) % COLORS_COUNT;
    pwm_set_brightness(physical_led, func_brightness[i]);
  }
}

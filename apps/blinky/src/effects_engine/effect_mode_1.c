#include "effects_internal.h"
#include "led.h"
#include "pwm_engine.h"
#include <stdint.h>

static uint8_t mode_1_shift = 0;

void effect_mode_1_short_press() {
  mode_1_shift = (mode_1_shift + 1) % COLORS_COUNT;
}

void effect_mode_1_update(uint32_t time_ms) {
  uint8_t func_brightness[COLORS_COUNT];

  func_brightness[0] =
      ((time_ms % 1000) < 500) ? PWM_MAX_BRIGHTNESS : PWM_MIN_BRIGHTNESS;
  func_brightness[1] =
      ((time_ms % 500) < 250) ? PWM_MAX_BRIGHTNESS : PWM_MIN_BRIGHTNESS;

  uint32_t t2 = time_ms % 2000;
  if (t2 < 1000) {
    func_brightness[2] = t2 * PWM_MAX_BRIGHTNESS / 1000;
  } else {
    func_brightness[2] = (2000 - t2) * PWM_MAX_BRIGHTNESS / 1000;
  }

  uint32_t t3 = time_ms % 1000;
  if (t3 < 500) {
    func_brightness[3] = (500 - t3) * PWM_MAX_BRIGHTNESS / 500;
  } else {
    func_brightness[3] = (t3 - 500) * PWM_MAX_BRIGHTNESS / 500;
  }

  for (int i = 0; i < COLORS_COUNT; i++) {
    int physical_led = (i + mode_1_shift) % COLORS_COUNT;
    pwm_set_brightness(physical_led, func_brightness[i]);
  }
}

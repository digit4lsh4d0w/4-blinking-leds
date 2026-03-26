#include "effects_internal.h"
#include "led.h"
#include "pwm_engine.h"
#include <stdint.h>

typedef enum : uint8_t {
  LOW,
  MID,
  HIGH,
  SPEEDS_COUNT,
} effect_mode_2_speeds;

static constexpr uint32_t speeds[] = {
    [LOW] = 1000,
    [MID] = 500,
    [HIGH] = 300,
};

static effect_mode_2_speeds current_speed = LOW;

static uint32_t last_step_time = 0;
static uint8_t active_led = 0;

void effect_mode_2_short_press() {
  current_speed = (current_speed + 1) % SPEEDS_COUNT;
}

void effect_mode_2_update(uint32_t time_ms) {
  if (time_ms - last_step_time >= speeds[current_speed] ||
      last_step_time == 0) {
    if (last_step_time != 0) {
      active_led = (active_led + 1) % COLORS_COUNT;
    }

    last_step_time = time_ms;
  }

  for (int i = 0; i < COLORS_COUNT; i++) {
    pwm_set_brightness(i, (i == active_led) ? PWM_MAX_BRIGHTNESS
                                            : PWM_MIN_BRIGHTNESS);
  }
}

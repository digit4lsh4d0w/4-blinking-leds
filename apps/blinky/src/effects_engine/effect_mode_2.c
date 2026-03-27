#include "effects_internal.h"
#include "led.h"
#include "pwm_engine.h"
#include <stdatomic.h>
#include <stdint.h>

typedef enum : uint8_t {
  EFFECT_MODE_2_SPEED_LOW,
  EFFECT_MODE_2_SPEED_MID,
  EFFECT_MODE_2_SPEED_HIGH,
  EFFECT_MODE_2_SPEED_COUNT,
} effect_mode_2_speeds;

static constexpr uint32_t speeds_ms[EFFECT_MODE_2_SPEED_COUNT] = {
    [EFFECT_MODE_2_SPEED_LOW] = 1000,
    [EFFECT_MODE_2_SPEED_MID] = 500,
    [EFFECT_MODE_2_SPEED_HIGH] = 300,
};

static _Atomic effect_mode_2_speeds current_speed = EFFECT_MODE_2_SPEED_LOW;
static uint32_t last_step_time = 0;
static uint8_t active_led = 0;

void effect_mode_2_short_press() {
  effect_mode_2_speeds speed = atomic_load(&current_speed);
  atomic_store(&current_speed, (speed + 1) % EFFECT_MODE_2_SPEED_COUNT);
}

void effect_mode_2_update(uint32_t time_ms) {
  effect_mode_2_speeds speed = atomic_load(&current_speed);
  uint32_t current_step_duration = time_ms - last_step_time;

  if (current_step_duration >= speeds_ms[speed] || last_step_time == 0) {
    if (last_step_time != 0) {
      active_led = (active_led + 1) % COLORS_COUNT;
    }

    last_step_time = time_ms;
  }

  for (uint8_t i = 0; i < COLORS_COUNT; i++) {
    pwm_set_brightness(i, (i == active_led) ? PWM_MAX_BRIGHTNESS
                                            : PWM_MIN_BRIGHTNESS);
  }
}

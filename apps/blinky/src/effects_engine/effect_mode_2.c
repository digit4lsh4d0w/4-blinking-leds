#include "effects_internal.h"
#include "led.h"
#include "pwm_engine.h"
#include <stdint.h>

typedef enum : uint8_t {
  LOW,
  MID,
  HIGH,
} effect_mode_2_speeds;

static constexpr uint32_t speeds[] = {
    [LOW] = 1000,
    [MID] = 500,
    [HIGH] = 300,
};

static uint32_t mode_2_speed_ms = speeds[LOW];

void effect_mode_2_short_press() {
  switch (mode_2_speed_ms) {
  case LOW:
    mode_2_speed_ms = speeds[MID];
    break;
  case MID:
    mode_2_speed_ms = speeds[HIGH];
    break;
  case HIGH:
    mode_2_speed_ms = speeds[LOW];
    break;
  }
}

void effect_mode_2_update(uint32_t time_ms) {
  uint8_t active_led = (time_ms / mode_2_speed_ms) % COLORS_COUNT;

  for (int i = 0; i < COLORS_COUNT; i++) {
    pwm_set_brightness(i, (i == active_led) ? PWM_MAX_BRIGHTNESS
                                            : PWM_MIN_BRIGHTNESS);
  }
}

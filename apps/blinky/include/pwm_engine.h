#pragma once

#include "include/led.h"
#include <stdint.h>

constexpr uint8_t PWM_MAX_BRIGHTNESS = 100;
constexpr uint8_t PWM_MIN_BRIGHTNESS = 0;

// Рабочий цикл программного ШИМ.
void pwm_engine_init();
// Установка яркости свечения светодиода.
void pwm_set_brightness(led_color_t color, uint8_t brightness);

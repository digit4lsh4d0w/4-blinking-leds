#pragma once

#include <stdint.h>

// Перечисление для цветов светодиодов.
typedef enum : uint8_t {
  LED_COLOR_RED = 0,
  LED_COLOR_GREEN,
  LED_COLOR_BLUE,
  LED_COLOR_ORANGE,
  LED_COLOR_COUNT,
} led_color_t;

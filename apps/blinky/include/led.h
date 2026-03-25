#pragma once

#include <stdint.h>

// Перечисление для цветов светодиодов.
typedef enum : uint8_t {
  RED = 0,
  GREEN,
  BLUE,
  ORANGE,
  COLORS_COUNT,
} led_color_t;

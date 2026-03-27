#pragma once

#include <stdint.h>

// Режимы мигания светодиодами.
typedef enum : uint8_t {
  EFFECT_MODE_1,
  EFFECT_MODE_2,
  EFFECT_MODES_COUNT,
} effect_mode_t;

// Рабочий цикл управления режимами свечения.
void effects_engine_init();
// Обработчик короткого нажатия на кнопку.
void effects_engine_on_short_press();
// Обработчик длинного нажатия на кнопку.
void effects_engine_on_long_press();

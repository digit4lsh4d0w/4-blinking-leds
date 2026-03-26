#pragma once

#include <stdint.h>

// Режимы мигания светодиодами.
typedef enum : uint8_t {
  MODE_1,
  MODE_2,
  MODES_COUNT,
} effect_mode_t;

void logic_engine_on_short_press();
void logic_engine_on_long_press();
// Рабочий цикл управления режимами свечения.
void logic_engine_init();
// Функция переключения режима свечения.
void logic_engine_next_mode();

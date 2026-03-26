#pragma once

#include <stdint.h>

// Режимы мигания светодиодами.
typedef enum : uint8_t {
  MODE_1,
  MODE_2,
  MODES_COUNT,
} effect_mode_t;

// Рабочий цикл управления режимами свечения.
void logic_engine_init();
// Функция переключения режима свечения.
void logic_engine_next_mode();

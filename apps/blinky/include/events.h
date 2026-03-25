#pragma once

#include <stdint.h>

// Типы событий кнопки
typedef enum : uint8_t {
  BTN_SHORT_PRESS,
  BTN_LONG_PRESS,
} btn_event_type_t;

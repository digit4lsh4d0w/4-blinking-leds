#include "board.h"
#include "cpu_conf_common.h"
#include "events.h"
#include "periph/cpu_gpio.h"
#include "periph/gpio.h"
#include "thread.h"
#include "thread_config.h"
#include "ztimer.h"
#include <stdatomic.h>
#include <stdint.h>

// Разрешение 100 означает, что скважность может быть одним из 100 значений.
// Удобно для счета в процентах, от 0% до 100%.
constexpr uint8_t PWM_RESOLUTION = 100;
// Шаг таймера в микросекундах.
// При шаге 100 мкс и разрешении 100 полный цикл ШИМ составит 10000 мкс / 10 мс,
// что дает частоту обновления 100 Гц.
// Человеческий глаз воспринимает частоту выше 24 Гц как анимацию, поэтому такой
// частоты будет достаточно.
constexpr uint8_t PWM_STEP_US = 100;

// Перечисление для цветов светодиодов.
typedef enum : uint8_t {
  RED = 0,
  GREEN,
  BLUE,
  ORANGE,
  COLORS_COUNT,
} led_color_t;

// Атомарный массив для межпоточного взаимодействия.
// Сюда другие потоки будут записывать значения интенсивности свечения
// светодиодов.
_Atomic uint8_t g_led_brightness[4] = {
    [RED] = 0,
    [GREEN] = 0,
    [BLUE] = 0,
    [ORANGE] = 0,
};

constexpr gpio_t led_pins[] = {
    [RED] = LED3_PIN,
    [GREEN] = LED5_PIN,
    [BLUE] = LED6_PIN,
    [ORANGE] = LED4_PIN,
};

// Выделение памяти под стек для отдельного потока ШИМ.
static char pwm_thread_stack[THREAD_STACKSIZE_DEFAULT];

// Рабочий цикл программного ШИМ.
static void *pwm_thread([[maybe_unused]] void *arg) {
  uint32_t last_wakeup = ztimer_now(ZTIMER_USEC);
  uint8_t counter = 0;

  while (true) {
    for (int i = RED; i < COLORS_COUNT; i++) {
      uint8_t current_brightness = atomic_load(&g_led_brightness[i]);
      if (counter < current_brightness) {
        gpio_set(led_pins[i]);
      } else {
        gpio_clear(led_pins[i]);
      }
    }

    counter++;
    if (counter >= PWM_RESOLUTION) {
      counter = 0;
    }

    ztimer_periodic_wakeup(ZTIMER_USEC, &last_wakeup, PWM_STEP_US);
  }

  return nullptr;
}

// Точка входа в цикл программного ШИМ.
void pwm_engine_init() {
  for (int i = RED; i < COLORS_COUNT; i++) {
    gpio_init(led_pins[i], GPIO_OUT);
  }

  // Создание потока с повышенным приоритетом.
  thread_create(pwm_thread_stack, sizeof(pwm_thread_stack),
                THREAD_PRIORITY_MAIN - 2, 0, pwm_thread, nullptr, "pwm");
}

#include "pwm_engine.h"
#include "board.h"
#include "cpu_conf_common.h"
#include "led.h"
#include "periph/cpu_gpio.h"
#include "periph/gpio.h"
#include "thread.h"
#include "thread_config.h"
#include "ztimer.h"
#include <stdatomic.h>
#include <stdint.h>

// Разрешение 100 означает, что скважность может быть одним из 100 значений.
// Удобно для счета в процентах, от 0% до 100%.
static constexpr uint8_t PWM_RESOLUTION = 100;
// Шаг таймера в микросекундах.
// При шаге 100 мкс и разрешении 100 полный цикл ШИМ составит 10000 мкс / 10 мс,
// что дает частоту обновления 100 Гц.
// Человеческий глаз воспринимает частоту выше 24 Гц как анимацию, поэтому такой
// частоты будет достаточно.
static constexpr uint8_t PWM_STEP_US = 100;

// Lookup table для установления соотвествия цвета и номера пина.
static constexpr gpio_t led_pins[] = {
    [LED_COLOR_RED] = LED3_PIN,
    [LED_COLOR_GREEN] = LED5_PIN,
    [LED_COLOR_BLUE] = LED6_PIN,
    [LED_COLOR_ORANGE] = LED4_PIN,
};

static char pwm_thread_stack[THREAD_STACKSIZE_DEFAULT];

// Переменная для хранения значений интенсивности свечения светодиодов.
static _Atomic uint8_t led_brightness[LED_COLOR_COUNT] = {
    [LED_COLOR_RED] = 0,
    [LED_COLOR_GREEN] = 0,
    [LED_COLOR_BLUE] = 0,
    [LED_COLOR_ORANGE] = 0,
};

// Поток RIOT OS для модуляции ШИМ сигнала.
static void *pwm_thread([[maybe_unused]] void *arg) {
  uint32_t last_wakeup = ztimer_now(ZTIMER_USEC);
  uint8_t duty_counter = 0;

  while (true) {
    for (uint8_t i = 0; i < LED_COLOR_COUNT; i++) {
      uint8_t current_brightness = atomic_load(&led_brightness[i]);
      if (duty_counter < current_brightness) {
        gpio_set(led_pins[i]);
      } else {
        gpio_clear(led_pins[i]);
      }
    }

    duty_counter++;
    if (duty_counter >= PWM_RESOLUTION) {
      duty_counter = 0;
    }

    ztimer_periodic_wakeup(ZTIMER_USEC, &last_wakeup, PWM_STEP_US);
  }

  return nullptr;
}

void pwm_set_brightness(led_color_t color, uint8_t brightness) {
  if (color < LED_COLOR_COUNT && brightness <= PWM_RESOLUTION) {
    atomic_store(&led_brightness[color], brightness);
  }
}

void pwm_engine_init() {
  // Инициализация светодиодов.
  for (uint8_t i = 0; i < LED_COLOR_COUNT; i++) {
    gpio_init(led_pins[i], GPIO_OUT);
  }

  thread_create(pwm_thread_stack, sizeof(pwm_thread_stack),
                THREAD_PRIORITY_MAIN - 2, 0, pwm_thread, nullptr, "pwm");
}

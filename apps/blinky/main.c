#include "board.h"
#include "periph/cpu_gpio.h"
#include "periph/gpio.h"
#include "periph/pwm.h"
#include "ztimer.h"
#include <stdint.h>
#include <stdio.h>

// Время ожидания между сменой состояний светодиода
// const uint32_t SLEEP_TIME_MS = 400;
const uint32_t STEP_DELAY_MS = 5;
const int16_t PWM_RESOLUTION = 1000;
const int16_t PWM_FREQ = 1000;
const int16_t INITIAL_STEP = 20;
const int16_t INITIAL_DUTY = 20;

typedef enum : uint8_t {
  LED_RED = 0,
  LED_GREEN,
  LED_BLUE,
  LED_ORANGE,
  LED_PINS_COUNT
} led_color_t;

// static const gpio_t LED_PINS[] = {
//     [LED_RED] = LED3_PIN,
//     [LED_GREEN] = LED5_PIN,
//     [LED_BLUE] = LED6_PIN,
//     [LED_ORANGE] = LED4_PIN,
// };

typedef struct {
  pwm_t dev;
  uint8_t chan;
} pwm_led_t;

// static const pwm_led_t PWM_LEDS[] = {
//     [LED3_PIN] = { .dev = PWM_DEV(1), .chan = 1 },
//     [LED4_PIN] = { .dev = PWM_DEV(1), .chan = 0 },
//     [LED5_PIN] = { .dev = PWM_DEV(2), .chan = 0 },
//     [LED6_PIN] = { .dev = PWM_DEV(2), .chan = 1 },
// };

// typedef struct {
//     _Atomic active_led_t current_state;
//     _Atomic active_breath_led_t current_breath_state;
// } app_ctx_t;

// Переключает состояние светодиода
// void blink(active_led_t state)
// {
//     gpio_t led_pin = LED_PINS[state];

//     gpio_set(led_pin);
//     ztimer_sleep(ZTIMER_MSEC, SLEEP_TIME_MS);
//     gpio_clear(led_pin);
//     ztimer_sleep(ZTIMER_MSEC, SLEEP_TIME_MS);
// }

// Циклически изменяет выбор текущего светодиода
// void switch_blink(void *void_ctx)
// {
//     app_ctx_t *ctx = void_ctx;

//     // active_led_t simple_led_next_state = (ctx->current_state + 1) %
//     LED_STATE_COUNT;
//     // ctx->current_state = simple_led_next_state;

//     active_breath_led_t breath_led_next_state = (ctx->current_breath_state +
//     1) % BREATH_LED_STATE_COUNT; ctx->current_breath_state =
//     breath_led_next_state;
// }

int main(void) {
  printf("Starting!\n");

  // pwm_init(PWM_DEV(1), PWM_LEFT, PWM_FREQ, PWM_RESOLUTION);
  // pwm_init(PWM_DEV(2), PWM_LEFT, PWM_FREQ, PWM_RESOLUTION);

  // pwm_set(PWM_DEV(1), 0, 0);
  // pwm_set(PWM_DEV(1), 1, 0);
  // pwm_set(PWM_DEV(2), 0, 0);
  // pwm_set(PWM_DEV(2), 1, 0);

  // // Создание контекста приложения
  // app_ctx_t ctx = {
  //     .current_breath_state = ACTIVE_BREATH_LED_3,
  // };

  // // Привязка прерывания при нажатии кнопки
  // // на вызов функции `switch_blink` с аргументом `&state`
  // gpio_init_int(BTN0_PIN, GPIO_IN, GPIO_FALLING, switch_blink, &ctx);

  // int16_t step = INITIAL_STEP;
  // int16_t current_duty = INITIAL_DUTY;

  // // Цикл мерцания светодиода
  // while (1) {
  //     uint8_t active_pair = ctx.current_breath_state % 2;

  //     pwm_t active_dev = (active_pair == 0) ? PWM_DEV(1) : PWM_DEV(2);
  //     pwm_t inactive_dev = (active_pair == 0) ? PWM_DEV(2) : PWM_DEV(1);

  //     pwm_set(inactive_dev, 0, 0);
  //     pwm_set(inactive_dev, 1, PWM_RESOLUTION);

  //     pwm_set(active_dev, 0, current_duty);
  //     pwm_set(active_dev, 1, PWM_RESOLUTION - current_duty);

  //     current_duty += step;

  //     if (current_duty >= PWM_RESOLUTION) {
  //         current_duty = PWM_RESOLUTION;
  //         step = -step;
  //     } else if (current_duty <= 0) {
  //         current_duty = 0;
  //         step = -step;
  //     }

  //     ztimer_sleep(ZTIMER_MSEC, STEP_DELAY_MS);
  // }

  return 0;
}

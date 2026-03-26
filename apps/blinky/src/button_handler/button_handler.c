#include "button_handler.h"
#include "effects_engine.h"
#include "msg.h"
#include "periph/cpu_gpio.h"
#include "periph/gpio.h"
#include "thread.h"
#include "thread_config.h"
#include "ztimer.h"
#include <stdint.h>

static constexpr uint32_t DEBOUNCE_MS = 50;
static constexpr uint32_t LONG_PRESS_MS = 1000;

static constexpr uint16_t MSG_BTN_PRESSED = 1;
static constexpr uint16_t MSG_BTN_RELEASED = 2;

static char btn_thread_stack[THREAD_STACKSIZE_MAIN];
static kernel_pid_t btn_thread_pid;

static void btn_isr(void *arg) {
  gpio_t pin = (gpio_t)arg;

  msg_t msg;
  msg.content.value = ztimer_now(ZTIMER_MSEC);

  if (gpio_read(pin) > 0) {
    msg.type = MSG_BTN_PRESSED;
  } else {
    msg.type = MSG_BTN_RELEASED;
  }

  msg_try_send(&msg, btn_thread_pid);
}

static void *button_thread([[maybe_unused]] void *arg) {
  msg_t msg_queue[8];
  msg_init_queue(msg_queue, 8);

  msg_t msg;
  uint32_t press_time = 0;
  bool is_pressed = false;

  while (true) {
    msg_receive(&msg);
    uint32_t event_time = msg.content.value;
    uint32_t duration = event_time - press_time;

    if (duration > DEBOUNCE_MS) {
      if (msg.type == MSG_BTN_PRESSED && !is_pressed) {
        press_time = event_time;
        is_pressed = true;
      } else if (msg.type == MSG_BTN_RELEASED && is_pressed) {
        is_pressed = false;
        press_time = event_time;
        if (duration >= LONG_PRESS_MS) {
          logic_engine_on_long_press();
        } else {
          logic_engine_on_short_press();
        }
      }
    }
  }

  return nullptr;
}

void button_handler_init(gpio_t btn_pin) {
  btn_thread_pid = thread_create(btn_thread_stack, sizeof(btn_thread_stack),
                                 THREAD_PRIORITY_MAIN - 1, 0, button_thread,
                                 nullptr, "button");

  gpio_init_int(btn_pin, GPIO_IN_PD, GPIO_BOTH, btn_isr, (void *)btn_pin);
}

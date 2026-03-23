# Архитектура программного ШИМ и управления эффектами в RIOT-OS

## Описание проблемы и выбор решения
Аппаратный ШИМ не подходит для независимого управления парами светодиодов, если они подключены к одному аппаратному таймеру, но к разным выходам (прямому и комплементарному). Для реализации независимых частот и скважностей требуется программный ШИМ.

Высокоуровневая абстракция SAUL неэффективна для программного ШИМ из-за высоких накладных расходов при частом вызове (сотни/тысячи раз в секунду). Оптимальное решение в рамках RIOT-OS — использование многопоточности (Threads) и системы обмена сообщениями (IPC).

С учетом требований к безопасности и современным практикам, код приведен к стандарту **C23**, что позволяет избежать типичных ошибок C (макросы, data races, нетипизированные указатели).

## План выполнения работы и архитектура

1. **Модуль ШИМ (Поток 1 - Высокий приоритет):**
   Отдельный поток, реализующий программный ШИМ. Выполняет только чтение массива требуемой яркости и управление GPIO через `ztimer_periodic_wakeup` (с микросекундной точностью).
2. **Модуль Логики (Поток 2 - Средний приоритет):**
   Конечный автомат (State Machine). Каждые 20-50 мс пересчитывает яркость каждого светодиода в зависимости от текущего режима, системного времени и логики эффектов.
3. **Модуль Кнопки (Прерывания + IPC):**
   Настройка прерывания на оба фронта (нажатие и отпускание). Измерение времени удержания для генерации событий `SHORT_PRESS` и `LONG_PRESS` через отправку сообщений (IPC) в поток логики.

## Структура мини-проекта

```text
/workspace/examples/stm32f3348-disco/advanced_blinky/
├── Makefile
├── main.c              (Точка входа, запуск потоков)
├── app_events.h        (Общие структуры событий и глобальные переменные)
├── pwm_engine.h/.c     (Поток программного ШИМ)
├── logic_engine.h/.c   (Поток вычисления эффектов и конечный автомат)
└── button_handler.h/.c (Обработка прерываний кнопки и таймингов)
```

## Примеры ключевых компонентов (Современный C23)

### 1. `app_events.h` (Общие события и разделяемая память)
Вместо классического `#ifndef` используется современный `#pragma once`. 
Ключевое слово `volatile` заменено на `_Atomic`, что гарантирует потокобезопасность (отсутствие data races) при чтении и записи значений между логикой и ШИМ.
```c
#pragma once

#include <stdint.h>

// Типы событий от кнопки (C23: строго типизированный enum для экономии памяти)
typedef enum : uint8_t {
    EVENT_BTN_SHORT_PRESS = 1,
    EVENT_BTN_LONG_PRESS  = 2,
} app_event_type_t;

// Глобальный массив текущей яркости (0-100) для 4 светодиодов.
// _Atomic гарантирует барьеры памяти и безопасное IPC между потоками.
extern _Atomic uint8_t g_led_brightness[4];
```

### 2. `pwm_engine.c` (Движок программного ШИМ)
Макросы `#define` заменены на типизированные `constexpr`. Вместо `while (1)` и `NULL` используются встроенные в C23 ключевые слова `true` и `nullptr`. Атрибут `[[maybe_unused]]` заменяет устаревший каст `(void)arg`.
```c
#include "thread.h"
#include "ztimer.h"
#include "periph/gpio.h"
#include "app_events.h"
#include <stdatomic.h>

// C23: типизированные константы времени компиляции
constexpr uint8_t  PWM_RESOLUTION = 100;
constexpr uint32_t PWM_STEP_US    = 100; // 100 мкс * 100 = 10 мс период (100 Гц)

_Atomic uint8_t g_led_brightness[4] = {0, 0, 0, 0};
static const gpio_t led_pins[4] = { LED3_PIN, LED5_PIN, LED6_PIN, LED4_PIN };
static char pwm_thread_stack[THREAD_STACKSIZE_DEFAULT];

// C23: атрибут [[maybe_unused]] для подавления варнингов
static void *pwm_thread([[maybe_unused]] void *arg) {
    uint32_t last_wakeup = ztimer_now(ZTIMER_USEC);
    uint8_t counter = 0;

    // C23: true теперь часть языка (не нужен <stdbool.h>)
    while (true) {
        for (int i = 0; i < 4; i++) {
            // Безопасное атомарное чтение
            uint8_t current_brightness = atomic_load(&g_led_brightness[i]);
            
            if (counter < current_brightness) {
                gpio_set(led_pins[i]);
            } else {
                gpio_clear(led_pins[i]);
            }
        }
        
        counter++;
        // В RTOS и прерываниях предпочтительнее if, чем деление по модулю (%)
        if (counter >= PWM_RESOLUTION) {
            counter = 0;
        }

        ztimer_periodic_wakeup(ZTIMER_USEC, &last_wakeup, PWM_STEP_US);
    }
    
    // C23: nullptr вместо макроса NULL
    return nullptr;
}

// C23: пустые скобки () строго означают функцию без аргументов, void не нужен
void pwm_engine_init() {
    for (int i = 0; i < 4; i++) { 
        gpio_init(led_pins[i], GPIO_OUT); 
    }
    
    // Приоритет выше обычного, чтобы системные задачи не вызывали jitter ШИМа
    thread_create(pwm_thread_stack, sizeof(pwm_thread_stack),
                  THREAD_PRIORITY_MAIN - 2, 0, pwm_thread, nullptr, "pwm");
}
```

### 3. `logic_engine.c` (Логика эффектов)
Запись в разделяемый массив производится через `atomic_store`, обеспечивая корректное межпоточное взаимодействие.
```c
#include "thread.h"
#include "msg.h"
#include "ztimer.h"
#include "app_events.h"
#include <stdatomic.h>

static char logic_thread_stack[THREAD_STACKSIZE_MAIN];
static msg_t msg_queue[8];

typedef enum : uint8_t { 
    MODE_1, 
    MODE_2 
} app_mode_t;

static app_mode_t current_mode = MODE_1;
static uint8_t mode1_shift = 0;

static void process_mode_1(uint32_t time_ms) {
    uint8_t target[4] = {0};
    
    // Функция 1: Моргает 1 раз в секунду
    target[0] = ((time_ms % 1000) < 500) ? 100 : 0;
    // Функция 2: Моргает 2 раза в секунду
    target[1] = ((time_ms % 500) < 250) ? 100 : 0;
    
    // Функция 3: Плавно загорается (1с) и гаснет (1с)
    uint32_t t3 = time_ms % 2000;
    target[2] = (t3 < 1000) ? (t3 * 100 / 1000) : ((2000 - t3) * 100 / 1000);
    
    // Функция 4: Плавно гаснет (0.5с) и загорается (0.5с)
    uint32_t t4 = time_ms % 1000;
    target[3] = (t4 < 500) ? (100 - (t4 * 100 / 500)) : (((t4 - 500) * 100) / 500);

    // Смещение функций по светодиодам
    for (int i = 0; i < 4; i++) {
        int physical_led = (i + mode1_shift) % 4;
        
        // Безопасная атомарная запись для передачи в поток ШИМ
        atomic_store(&g_led_brightness[physical_led], target[i]);
    }
}

static void *logic_thread([[maybe_unused]] void *arg) {
    msg_init_queue(msg_queue, 8);
    uint32_t last_wakeup = ztimer_now(ZTIMER_MSEC);
    
    constexpr uint32_t LOGIC_PERIOD_MS = 20; // 50 FPS

    while (true) {
        msg_t msg;
        if (msg_try_receive(&msg) == 1) {
            // Обработка сообщений от кнопки (смена режимов)
        }

        uint32_t current_time_ms = ztimer_now(ZTIMER_MSEC);
        if (current_mode == MODE_1) {
            process_mode_1(current_time_ms);
        } else {
            // Логика режима 2 (бегущий огонь)
        }

        ztimer_periodic_wakeup(ZTIMER_MSEC, &last_wakeup, LOGIC_PERIOD_MS);
    }
    
    return nullptr;
}
```

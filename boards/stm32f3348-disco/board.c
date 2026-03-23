/*
 * Copyright (C) 2021 Borovick Alexey
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     boards_stm32f3348-disco
 * @{
 *
 * @file
 * @brief       Board specific implementations for the stm32f3348-disco evaluation board
 *
 * @author      Borovik Alexey <alexey@borovik.me>
 *
 * @}
 */

#include "board.h"

void board_init() {
    gpio_init(LED3_PIN, GPIO_OUT);
    gpio_init(LED4_PIN, GPIO_OUT);
    gpio_init(LED5_PIN, GPIO_OUT);
    gpio_init(LED6_PIN, GPIO_OUT);
}

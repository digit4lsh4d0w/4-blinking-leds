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
 * @brief       Board specific definitions for the stm32f3348-disco evaluation board
 *
 * @author      Borovik Alexey <alexey@borovik.me>
 *
 * @}
 */

#pragma once

#include "periph/uart.h"
#include "periph/spi.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @name Macros for controlling the on-board LEDs.
 * @{
 */
#define LED_PORT GPIOB

constexpr auto LED3_PIN = GPIO_PIN(PORT_B, 6);
constexpr auto LED4_PIN = GPIO_PIN(PORT_B, 8);
constexpr auto LED5_PIN = GPIO_PIN(PORT_B, 9);
constexpr auto LED6_PIN = GPIO_PIN(PORT_B, 7);
/** @} */

/**
 * @name User button
 * @{
 */
constexpr auto BTN0_PIN = GPIO_PIN(PORT_A, 0);
constexpr auto BTN0_MODE = GPIO_IN;
/** @} */

/**
 * @name Default UART configuration
 * @{
 */
constexpr auto STDIO_UART_DEV = UART_DEV(1);
/** @} */

/**
 * @name SPI SDCard settings
 * @{
 */
constexpr auto SDCARD_SPI_PARAM_SPI = SPI_DEV(0);
constexpr auto SDCARD_SPI_PARAM_CS = (GPIO_PIN(PORT_B, 10));
constexpr auto SDCARD_SPI_PARAM_CLK = (GPIO_PIN(PORT_A, 5));
constexpr auto SDCARD_SPI_PARAM_MOSI = (GPIO_PIN(PORT_A, 7));
constexpr auto SDCARD_SPI_PARAM_MISO = (GPIO_PIN(PORT_A, 6));
/** @} */

/**
 * @brief Initialize board specific hardware, including clock, LEDs and std-IO
 */
void board_init();

#ifdef __cplusplus
}
#endif

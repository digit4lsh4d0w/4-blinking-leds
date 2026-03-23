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
 * @brief       Peripheral MCU configuration for the stm32f3348-disco board
 *
 * @author      Borovik Alexey <alexey@borovik.me>
 *
 * @}
 */

#pragma once

/* This board provides an HSE */
#ifndef CONFIG_BOARD_HAS_HSE
#define CONFIG_BOARD_HAS_HSE    1
#endif

#include "periph_cpu.h"
#include "clk_conf.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @name   DAC configuration
 * @{
 */
constexpr dac_conf_t dac_config[] = {
    {
        .pin = GPIO_PIN(PORT_A,  4),
        .chan = 0
    }
};

#define DAC_NUMOF           ARRAY_SIZE(dac_config)
/** @} */

/**
 * @name   Timer configuration
 * @{
 */
static const timer_conf_t timer_config[] = {
    {
        .dev      = TIM2,
        .max      = 0xffffffff,
        .rcc_mask = RCC_APB1ENR_TIM2EN,
        .bus      = APB1,
        .irqn     = TIM2_IRQn
    }
};

#define TIMER_0_ISR         isr_tim2

#define TIMER_NUMOF         ARRAY_SIZE(timer_config)
/** @} */

/**
 * @name   UART configuration
 * @{
 */
static const uart_conf_t uart_config[] = {
    {
        .dev        = USART1,
        .rcc_mask   = RCC_APB2ENR_USART1EN,
        .rx_pin     = GPIO_PIN(PORT_A, 10),
        .tx_pin     = GPIO_PIN(PORT_A,  9),
        .rx_af      = GPIO_AF7,
        .tx_af      = GPIO_AF7,
        .bus        = APB2,
        .irqn       = USART1_IRQn
    },
    {
        .dev        = USART2,
        .rcc_mask   = RCC_APB1ENR_USART2EN,
        .rx_pin     = GPIO_PIN(PORT_B, 4),
        .tx_pin     = GPIO_PIN(PORT_B, 3),
        .rx_af      = GPIO_AF7,
        .tx_af      = GPIO_AF7,
        .bus        = APB1,
        .irqn       = USART2_IRQn
    },
    {
        .dev        = USART3,
        .rcc_mask   = RCC_APB1ENR_USART3EN,
        .rx_pin     = GPIO_PIN(PORT_C, 11),
        .tx_pin     = GPIO_PIN(PORT_C, 10),
        .rx_af      = GPIO_AF7,
        .tx_af      = GPIO_AF7,
        .bus        = APB1,
        .irqn       = USART3_IRQn
    }
};

#define UART_0_ISR          (isr_usart1)
#define UART_1_ISR          (isr_usart2)
#define UART_2_ISR          (isr_usart3)

#define UART_NUMOF          ARRAY_SIZE(uart_config)
/** @} */

/**
 * @name   ADC configuration
 *
 * We need to define the following fields:
 * PIN, device (ADCx), channel
 * @{
 */
constexpr adc_conf_t adc_config[] = {
    {
        .pin = GPIO_PIN(PORT_B, 14),
        .dev = 1,
        .chan = 14
    },
    {
        .pin = GPIO_PIN(PORT_B, 15),
        .dev = 1,
        .chan = 15
    },
    {
        .pin = GPIO_UNDEF,
        .dev = 1,
        .chan = 18
    }, /* VBAT */
};

#define VBAT_ADC            ADC_LINE(2) /**< VBAT ADC line */
#define ADC_NUMOF           ARRAY_SIZE(adc_config)
/** @} */

/**
 * @name    PWM configuration
 * @{
 */
static const pwm_conf_t pwm_config[] = {
    {
        .dev      = TIM3,
        .rcc_mask = RCC_APB1ENR_TIM3EN,
        .chan     = {
            { .pin = GPIO_PIN(PORT_C, 6), .cc_chan = 0 },
            { .pin = GPIO_PIN(PORT_C, 7), .cc_chan = 1 },
            { .pin = GPIO_PIN(PORT_C, 8), .cc_chan = 2 },
            { .pin = GPIO_PIN(PORT_C, 9), .cc_chan = 3 }
        },
        .af       = GPIO_AF2,
        .bus      = APB1
    },
    {
        .dev      = TIM16,
        .rcc_mask = RCC_APB2ENR_TIM16EN,
        .chan     = {
            { .pin = GPIO_PIN(PORT_B, 8), .cc_chan = 0 },
            { .pin = GPIO_PIN(PORT_B, 6), .cc_chan = 4 },
            { .pin = GPIO_UNDEF,          .cc_chan = 0 },
            { .pin = GPIO_UNDEF,          .cc_chan = 0 }
        },
        .af       = GPIO_AF1,
        .bus      = APB2
    },
    {
        .dev      = TIM17,
        .rcc_mask = RCC_APB2ENR_TIM17EN,
        .chan     = {
            { .pin = GPIO_PIN(PORT_B, 9), .cc_chan = 0 },
            { .pin = GPIO_PIN(PORT_B, 7), .cc_chan = 4 },
            { .pin = GPIO_UNDEF,          .cc_chan = 0 },
            { .pin = GPIO_UNDEF,          .cc_chan = 0 }
        },
        .af       = GPIO_AF1,
        .bus      = APB2
    }
};

#define PWM_NUMOF           ARRAY_SIZE(pwm_config)
/** @} */

/**
 * @name SPI configuration
 * @{
 */
static const spi_conf_t spi_config[] = {
    {
        .dev      = SPI1,
        .mosi_pin = GPIO_PIN(PORT_A, 7),
        .miso_pin = GPIO_PIN(PORT_A, 6),
        .sclk_pin = GPIO_PIN(PORT_A, 5),
        .cs_pin   = GPIO_UNDEF,
        .mosi_af  = GPIO_AF5,
        .miso_af  = GPIO_AF5,
        .sclk_af  = GPIO_AF5,
        .cs_af    = GPIO_AF5,
        .rccmask  = RCC_APB2ENR_SPI1EN,
        .apbbus   = APB2
    }
};

#define SPI_NUMOF           ARRAY_SIZE(spi_config)
/** @} */

#ifdef __cplusplus
}
#endif

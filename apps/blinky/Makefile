# Name of your application
APPLICATION = blinky

# If no BOARD is found in the environment, use this default:
BOARD ?= stm32f3348-disco

# Used modules
USEMODULE += periph_gpio
USEMODULE += periph_gpio_irq
USEMODULE += periph_pwm
USEMODULE += ztimer
USEMODULE += ztimer_msec

PROJECT_BASE ?= $(CURDIR)/../..
RIOTBASE ?= $(PROJECT_BASE)/RIOT

EXTERNAL_BOARD_DIRS += $(PROJECT_BASE)/boards

CFLAGS += -std=c23

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
DEVELHELP ?= 1

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

include $(RIOTBASE)/Makefile.include

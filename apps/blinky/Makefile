APPLICATION = blinky
BOARD ?= stm32f3348-disco

PROJECT_BASE ?= $(CURDIR)/../..
RIOTBASE ?= $(PROJECT_BASE)/RIOT

EXTERNAL_BOARD_DIRS += $(PROJECT_BASE)/boards
EXTERNAL_MODULE_DIRS += $(CURDIR)/src/
INCLUDES += -I$(CURDIR)/include

USEMODULE += pwm_engine
USEMODULE += effects_engine
USEMODULE += button_handler
USEMODULE += ztimer
USEMODULE += ztimer_msec
USEMODULE += ztimer_usec
USEMODULE += periph_gpio
USEMODULE += periph_gpio_irq
USEMODULE += periph_pwm

CFLAGS += -std=c23

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
DEVELHELP ?= 1

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

include $(RIOTBASE)/Makefile.include

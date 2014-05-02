#ifndef GPIO_H
#define GPIO_H

#if defined(bbb)
#include "bbb_gpio.h"
#elif defined(rpi)
#include "rpi_gpio.h"
#else
#error "No board defined. Edit Makefile."
#endif

#endif // GPIO_H


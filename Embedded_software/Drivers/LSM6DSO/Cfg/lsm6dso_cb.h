#ifndef __LSM6DSO_CB_H_
#define __LSM6DSO_CB_H_

#include "gpio.h"

void LSM6DSO_CallbackInt1(const struct device *dev, struct gpio_callback *cb, uint32_t pins);
void LSM6DSO_CallbackInt2(const struct device *dev, struct gpio_callback *cb, uint32_t pins);

#endif
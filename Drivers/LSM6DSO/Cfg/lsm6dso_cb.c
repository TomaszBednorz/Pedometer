#include "lsm6dso_cb.h"
#include "lsm6dso_tasks.h"


/* LSM6DSO callbacks */

void LSM6DSO_CallbackInt1(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
    k_wakeup(Lsm6dso_IdThreadInt1);
}

void LSM6DSO_CallbackInt2(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
    k_wakeup(Lsm6dso_IdThreadInt2);
}
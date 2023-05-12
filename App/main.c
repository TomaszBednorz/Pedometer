/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>

#include "gpio.h"
#include "spi.h"
#include "lsm6dso.h"
#include "ble.h"

void main(void)
{
	printk("Hello World! %s\n", CONFIG_BOARD);

	Gpio_Init();
	Spi_Init();

	LSM6DSO_Init();

	BLE_Init();




}


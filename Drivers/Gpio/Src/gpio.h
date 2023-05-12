#ifndef __GPIO_H_
#define __GPIO_H_

#include <zephyr/drivers/gpio.h>

#define GPIO_ON     (1U)
#define GPIO_OFF    (0U)

#define GPIO_LED1   (0U)
#define GPIO_LED2   (1U)
#define GPIO_LED3   (2U)
#define GPIO_LED4   (3U)

/* 
 * Function prototypes
 */
void Gpio_Init(void);
void Gpio_LedWrite(uint8_t led, int state);

#endif
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>

#include "gpio.h"
#include "lsm6dso_cb.h"

/*
 * Defines
 */
#define GPIO_LED_1    DT_ALIAS(led0)
#define GPIO_LED_2    DT_ALIAS(led1)
#define GPIO_LED_3    DT_ALIAS(led2)
#define GPIO_LED_4    DT_ALIAS(led3)

#define GPIO_LEDS_NUM   (4U)

#define GPIO_INT_1    DT_ALIAS(lsm6dsoint1)
#define GPIO_INT_2    DT_ALIAS(lsm6dsoint2)

#define GPIO_INTS_NUM   (2U)

/*
 * Local configuration objects
 */
static struct gpio_dt_spec Gpio_Leds[GPIO_LEDS_NUM] = { 
        GPIO_DT_SPEC_GET_OR(GPIO_LED_1, gpios, {0}),
        GPIO_DT_SPEC_GET_OR(GPIO_LED_2, gpios, {0}),
        GPIO_DT_SPEC_GET_OR(GPIO_LED_3, gpios, {0}),
        GPIO_DT_SPEC_GET_OR(GPIO_LED_4, gpios, {0})
    };

static struct gpio_dt_spec Gpio_IntPins[GPIO_INTS_NUM] = { 
        GPIO_DT_SPEC_GET_OR(GPIO_INT_1, gpios, {0}),
        GPIO_DT_SPEC_GET_OR(GPIO_INT_2, gpios, {0})
    };

static struct gpio_callback Gpio_IntPinsCb[GPIO_INTS_NUM];

/*!	
 * \brief GPIO initialization function
 *
 * \param[in] None
 *
 * \retval None
 */
void Gpio_Init(void)
{
    int ret;
	uint8_t i;

	for(i = 0; i < GPIO_LEDS_NUM; i++)
	{
		if (!device_is_ready(Gpio_Leds[i].port)) 
		{
			printk("Error: port device %s is not ready\r\n", Gpio_Leds[i].port->name);
			return;
		}
		if ((ret=gpio_pin_configure_dt(&Gpio_Leds[i], GPIO_OUTPUT)) != 0) 
		{
			printk("Error %d: failed to configure %s pin %d\n",
			ret, Gpio_Leds[i].port->name, Gpio_Leds[i].pin);
			return;
		}

        Gpio_LedWrite(i, GPIO_OFF);
	}

	for(i = 0; i < GPIO_INTS_NUM; i++)
	{
		if (!device_is_ready(Gpio_IntPins[i].port)) 
		{
			printk("Error: port device %s is not ready\r\n", Gpio_Leds[i].port->name);
			return;
		}
		if ((ret=gpio_pin_configure_dt(&Gpio_IntPins[i], GPIO_INPUT)) != 0) 
		{
			printk("Error %d: failed to configure %s pin %d\n",
			ret, Gpio_IntPins[i].port->name, Gpio_IntPins[i].pin);
			return;
		}
		if ((ret=gpio_pin_interrupt_configure_dt(&Gpio_IntPins[i], GPIO_INT_EDGE_TO_ACTIVE)) != 0) 
		{
			printk("Error %d: failed to configure %s pin %d\n",
			ret, Gpio_IntPins[i].port->name, Gpio_IntPins[i].pin);
			return;
		}
	}

    gpio_init_callback(&Gpio_IntPinsCb[0], LSM6DSO_CallbackInt1, BIT(Gpio_IntPins[0].pin));
    gpio_add_callback(Gpio_IntPins[0].port, &Gpio_IntPinsCb[0]);
    gpio_init_callback(&Gpio_IntPinsCb[1], LSM6DSO_CallbackInt2, BIT(Gpio_IntPins[1].pin));
    gpio_add_callback(Gpio_IntPins[1].port, &Gpio_IntPinsCb[1]);

}

/*!	
 * \brief GPIO initialization function
 *
 * \param[in] led LED number
 * \param[in] state LED state: GPIO_ON or GPIO_OFF
 * 
 * \retval None
 */
void Gpio_LedWrite(uint8_t led, int state)
{
    gpio_pin_set_dt(&Gpio_Leds[led], state);
}
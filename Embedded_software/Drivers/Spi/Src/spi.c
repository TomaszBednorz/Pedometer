#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/spi.h>

#include "spi.h"
#include "spi_cfg.h"


/*
 * Configuration data
 */
#define SPI1_NODE DT_NODELABEL(spi1)
const struct device *const dev = DEVICE_DT_GET(SPI1_NODE);

#define SPI1_CS_NODE DT_ALIAS(lsm6dsocs)
static const struct gpio_dt_spec Spi_Cs = GPIO_DT_SPEC_GET(SPI1_CS_NODE, gpios);

struct spi_config Spi_Config;

static uint8_t buffer_tx[SPI_MAX_BUF_LEN_TX];
static uint8_t buffer_rx[SPI_MAX_BUF_LEN_RX];

static struct spi_buf tx_buf[1] = 
{
    {
        .buf = buffer_tx, 
        .len = SPI_MAX_BUF_LEN_TX
    }
};

static struct spi_buf rx_buf[1] = 
{
    {
        .buf = buffer_rx, 
        .len = SPI_MAX_BUF_LEN_RX
    }
};

static struct spi_buf_set Spi_TxSet = { .buffers = tx_buf, .count = 1 };
static struct spi_buf_set Spi_RxSet = { .buffers = rx_buf, .count = 1 };

/*!	
 * \brief SPI initialization function
 *
 * \param[in] None
 *
 * \retval None
 */
void Spi_Init(void)
{
    int ret;

    if (!device_is_ready(Spi_Cs.port)) 
    {
        printk("Error: port device %s is not ready\r\n", Spi_Cs.port->name);
        return;
    }

    if ((ret=gpio_pin_configure_dt(&Spi_Cs, GPIO_OUTPUT)) != 0) 
    {
        printk("Error %d: failed to configure %s pin %d\n",
        ret, Spi_Cs.port->name, Spi_Cs.pin);
        return;
    }

    Spi_Config.frequency = SPI_FREQUENCY;
    Spi_Config.operation = SPI_OP_MODE_MASTER | SPI_WORD_SET(SPI_WROD_SIZE);
    Spi_Config.slave = SPI_OP_MODE_MASTER;

    gpio_pin_set_dt(&Spi_Cs, SPI_CS_DISABLE);
}

/*!	
 * \brief SPI transmission/reception function
 *
 * \param[in] length Amount of bytes to transfer
 * \param[in] tx_buffer Transmit data buffer
 * \param[in] rx_buffer Receive data buffer
 * 
 * \retval None
 */
void Spi_TxRx(uint8_t length, uint8_t* tx_buffer, uint8_t* rx_buffer)
{
    for(uint8_t i = 0; i < length; i++)
    {
        buffer_tx[i] = tx_buffer[i];
    }

    tx_buf[0].len = length;
    rx_buf[0].len = length;

    gpio_pin_set_dt(&Spi_Cs, SPI_CS_ENABLE);
    spi_transceive(dev, &Spi_Config, &Spi_TxSet, &Spi_RxSet);
    gpio_pin_set_dt(&Spi_Cs, SPI_CS_DISABLE);

    for(uint8_t i = 0; i < length; i++)
    {
        rx_buffer[i] = buffer_rx[i];
    }
}
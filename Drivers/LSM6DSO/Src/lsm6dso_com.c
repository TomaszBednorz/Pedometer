#include "lsm6dso_com.h"
#include "lsm6dso_cfg.h"

#define LSM6DSO_ADDR_MASK               (0x7F)

#define LSM6DSO_READ_MODE               (0x80)
#define LSM6DSO_WRITE_MODE              (0x00)

#define LSM6DSO_SINGLE_RW_LEN           (2U)

/*!	
 * \brief LSM6DSO single read function
 *
 * \param[in] address Register address
 * 
 * \retval Value of the register
 */
uint8_t LSM6DSO_SingleRead(uint8_t address)
{
    uint8_t tx_data[LSM6DSO_SINGLE_RW_LEN];
    uint8_t rx_data[LSM6DSO_SINGLE_RW_LEN];

    tx_data[0] = (address & LSM6DSO_ADDR_MASK) | LSM6DSO_READ_MODE;
    tx_data[1] = 0x00;

    LSM6DSO_ExchangeData(LSM6DSO_SINGLE_RW_LEN, tx_data, rx_data);

    return rx_data[1];
}

/*!	
 * \brief LSM6DSO single write function
 *
 * \param[in] address Register address
 * \param[in] data Value to store in the register
 * 
 * \retval None
 */
void LSM6DSO_SingleWrite(uint8_t address, uint8_t data)
{
    uint8_t tx_data[LSM6DSO_SINGLE_RW_LEN];
    uint8_t rx_data[LSM6DSO_SINGLE_RW_LEN];

    tx_data[0] = (address & LSM6DSO_ADDR_MASK) | LSM6DSO_WRITE_MODE;
    tx_data[1] = data;

    LSM6DSO_ExchangeData(LSM6DSO_SINGLE_RW_LEN, tx_data, rx_data);
}

/*!	
 * \brief LSM6DSO multiple read function
 *
 * \param[in] address First register address
 * \param[in] lenght Number of registers to read
 * \param[in] buf Buffer to store read data
 * 
 * \retval None
 */
void LSM6DSO_MultipleRead(uint8_t address, uint8_t length, uint8_t* buf)
{
    const uint8_t communication_length = length + 1U;
    uint8_t tx_data[communication_length];
    uint8_t rx_data[communication_length];

    tx_data[0] = (address & LSM6DSO_ADDR_MASK) | LSM6DSO_READ_MODE;

    for(uint8_t i = 1U; i < communication_length; i++)
    {
        tx_data[i] = 0x00;
    }

    LSM6DSO_ExchangeData(communication_length, tx_data, rx_data);

    for(uint8_t i = 0U; i < length; i++)
    {
        buf[i] = rx_data[i + 1U];
    }
}

/*!	
 * \brief LSM6DSO multiple write function
 *
 * \param[in] address First register address
 * \param[in] lenght Number of registers to write
 * \param[in] buf Buffer with data to write
 * 
 * \retval None
 */
void LSM6DSO_MultipleWrite(uint8_t address, uint8_t length, uint8_t* buf)
{
    const uint8_t communication_length = length + 1U;
    uint8_t tx_data[communication_length];
    uint8_t rx_data[communication_length];

    tx_data[0] = (address & LSM6DSO_ADDR_MASK) | LSM6DSO_WRITE_MODE;

    for(uint8_t i = 0U; i < length; i++)
    {
        tx_data[i + 1] = buf[i];
    }

    LSM6DSO_ExchangeData(communication_length, tx_data, rx_data);
}

#ifndef __LSM6DSO_CFG_H_
#define __LSM6DSO_CFG_H_

#include "spi.h"

#define LSM6DSO_ExchangeData(len, rx_data, tx_data)     (Spi_TxRx(len, rx_data, tx_data))

#define LSM6DSO_INT1_CTRL_CONFIG       (0x00)
#define LSM6DSO_INT2_CTRL_CONFIG       (0x07)
#define LSM6DSO_INT1_ROUTING_CONFIG    (0x02)
#define LSM6DSO_INT2_ROUTING_CONFIG    (0x00)
#define LSM6DSO_ACC_HIGH_RESOLUTION    (0x00)
#define LSM6DSO_ACC_FULL_SCALE         (0x02)  /* 4g */
#define LSM6DSO_ACC_ODR                (0x04)  /* 104 Hz */
#define LSM6DSO_GYRO_125DPS            (0x00)
#define LSM6DSO_GYRO_FULL_SCALE        (0x01)  /* 500 dps */
#define LSM6DSO_GYRO_ODR               (0x04)  /* 104 Hz */

#endif
#ifndef __BLE_H_
#define __BLE_H_

#include <zephyr/types.h>


/*
 * UUID (Universally Unique Identifier) definisions
 */
#define BLE_UUID_SERVICE              BT_UUID_128_ENCODE(0x00001523, 0x1112, 0xefde, 0x1523, 0x785feabcd123)
#define BLE_UUID_ACCELERATION_CHAR    BT_UUID_128_ENCODE(0x00001524, 0x1112, 0xefde, 0x1523, 0x785feabcd123)
#define BLE_UUID_ANGULAR_RATE_CHAR    BT_UUID_128_ENCODE(0x00001525, 0x1112, 0xefde, 0x1523, 0x785feabcd123)
#define BLE_UUID_TEMPERATURE_CHAR     BT_UUID_128_ENCODE(0x00001526, 0x1112, 0xefde, 0x1523, 0x785feabcd123)
#define BLE_UUID_STEPS_CHAR           BT_UUID_128_ENCODE(0x00001527, 0x1112, 0xefde, 0x1523, 0x785feabcd123)

#define BLE_UUID_VAL                  BT_UUID_DECLARE_128(BLE_UUID_SERVICE)
#define BLE_UUID_ACCELERATION_VAL     BT_UUID_DECLARE_128(BLE_UUID_ACCELERATION_CHAR)
#define BLE_UUID_ANGULAR_RATE_VAL     BT_UUID_DECLARE_128(BLE_UUID_ANGULAR_RATE_CHAR)
#define BLE_UUID_TEMPERATURE_VAL      BT_UUID_DECLARE_128(BLE_UUID_TEMPERATURE_CHAR)
#define BLE_UUID_STEPS_VAL            BT_UUID_DECLARE_128(BLE_UUID_STEPS_CHAR)

/*
 * Available characteristics
 */
typedef enum
{
    BLE_CHAR_ACCELERATION = 0,
    BLE_CHAR_ANGULAR_RATE,
    BLE_CHAR_TEMPERATURE,
    BLE_CHAR_STEPS,
    BLE_CHAR_MAX
}BLE_Characteristic_t;

/*
 * Functions prototypes
 */
void BLE_Init(void);

#endif
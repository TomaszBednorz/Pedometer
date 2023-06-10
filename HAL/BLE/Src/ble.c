#include "ble.h"
#include "pedometer.h"
#include "gpio.h"

#include <zephyr/kernel.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/gatt.h>

#define BLE_THREADS_STACKSIZE (512U)
#define BLE_THREAD_PRIO (9U)

#define DEVICE_NAME             CONFIG_BT_DEVICE_NAME
#define DEVICE_NAME_LEN         (sizeof(DEVICE_NAME) - 1)

#define BLE_CON_STATUS_LED          (GPIO_LED1)

/*
 * Static functions prototypes
 */
static void BLE_acceleration_status_changed(const struct bt_gatt_attr *attr, uint16_t value);
static void BLE_angular_rate_status_changed(const struct bt_gatt_attr *attr, uint16_t value);
static void BLE_temperature_status_changed(const struct bt_gatt_attr *attr, uint16_t value);
static void BLE_steps_status_changed(const struct bt_gatt_attr *attr, uint16_t value);
static void on_connected(struct bt_conn *conn, uint8_t err);
static void on_disconnected(struct bt_conn *conn, uint8_t reason);


/* 
 * BLE Service Declaration 
 */
BT_GATT_SERVICE_DEFINE(ble_service,
BT_GATT_PRIMARY_SERVICE(BLE_UUID_VAL),
	BT_GATT_CHARACTERISTIC(BLE_UUID_ACCELERATION_VAL,
			       BT_GATT_CHRC_NOTIFY,
			       BT_GATT_PERM_NONE, NULL, NULL,
			       NULL),
	BT_GATT_CCC(BLE_acceleration_status_changed,
		    BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CHARACTERISTIC(BLE_UUID_ANGULAR_RATE_VAL,
			       BT_GATT_CHRC_NOTIFY,
			       BT_GATT_PERM_NONE, NULL, NULL,
			       NULL),
	BT_GATT_CCC(BLE_angular_rate_status_changed,
		    BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
 	BT_GATT_CHARACTERISTIC(BLE_UUID_TEMPERATURE_VAL,
			       BT_GATT_CHRC_NOTIFY,
			       BT_GATT_PERM_NONE, NULL, NULL,
			       NULL),
	BT_GATT_CCC(BLE_temperature_status_changed,
		    BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CHARACTERISTIC(BLE_UUID_STEPS_VAL,
			       BT_GATT_CHRC_NOTIFY,
			       BT_GATT_PERM_NONE, NULL, NULL,
			       NULL),
	BT_GATT_CCC(BLE_steps_status_changed,
		    BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
);

/*
 * Local objects
 */
static uint8_t char_notify_status[BLE_CHAR_MAX];

static struct bt_le_adv_param *adv_param = BT_LE_ADV_PARAM((BT_LE_ADV_OPT_CONNECTABLE|BT_LE_ADV_OPT_USE_IDENTITY), /* Connectable advertising and use identity address */
                800, /*Min Advertising Interval 500ms (800*0.625ms) */
                801, /*Max Advertising Interval 500.625ms (801*0.625ms)*/
                NULL); /* Set to NULL for undirected advertising*/

static const struct bt_data ad[] = {
	BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
	BT_DATA(BT_DATA_NAME_COMPLETE, DEVICE_NAME, DEVICE_NAME_LEN),

};

static const struct bt_data sd[] = {
	BT_DATA_BYTES(BT_DATA_UUID128_ALL, BLE_UUID_SERVICE),
};

struct bt_conn_cb connection_callbacks = {
    .connected              = on_connected,
    .disconnected           = on_disconnected,  
};

/*!	
 * \brief BLE initialization function
 *
 * \param[in] None
 *
 * \retval None
 */
void BLE_Init(void)
{
    bt_enable(NULL);
    bt_conn_cb_register(&connection_callbacks);
    bt_le_adv_start(adv_param, ad, ARRAY_SIZE(ad), sd, ARRAY_SIZE(sd));
}

/*!	
 * \brief Task to handle BLE notificcations
 *
 * \param[in] None
 *
 * \retval None
 */
void BLE_Thread(void)
{
    Pedometer_Results_t data;

    while(1)
    {
        k_msleep(250);

        Pedometer_GetResults(&data);

        if(char_notify_status[BLE_CHAR_ACCELERATION])
        {
            bt_gatt_notify(NULL, &ble_service.attrs[2], &data.acceleration, sizeof(data.acceleration));
        }

        if(char_notify_status[BLE_CHAR_ANGULAR_RATE])
        {
           bt_gatt_notify(NULL, &ble_service.attrs[4], &data.angular_rate, sizeof(data.angular_rate));
        }

        if(char_notify_status[BLE_CHAR_TEMPERATURE])
        {
            bt_gatt_notify(NULL, &ble_service.attrs[7], &data.temperature, sizeof(data.temperature));
        }

        if(char_notify_status[BLE_CHAR_STEPS])
        {
           bt_gatt_notify(NULL, &ble_service.attrs[10], &data.steps, sizeof(data.steps));
        }
    }
}

/*
 * Callback for acceleration configuration change
 */
static void BLE_acceleration_status_changed(const struct bt_gatt_attr *attr, uint16_t value)
{
    char_notify_status[BLE_CHAR_ACCELERATION] = (value == BT_GATT_CCC_NOTIFY);
}

/*
 * Callback for angfular rate configuration change
 */
static void BLE_angular_rate_status_changed(const struct bt_gatt_attr *attr, uint16_t value)
{
    char_notify_status[BLE_CHAR_ANGULAR_RATE] = (value == BT_GATT_CCC_NOTIFY);
}

/*
 * Callback for temperature configuration change
 */
static void BLE_temperature_status_changed(const struct bt_gatt_attr *attr, uint16_t value)
{
    char_notify_status[BLE_CHAR_TEMPERATURE] = (value == BT_GATT_CCC_NOTIFY);
}

/*
 * Callback for steps configuration change
 */
static void BLE_steps_status_changed(const struct bt_gatt_attr *attr, uint16_t value)
{
    char_notify_status[BLE_CHAR_STEPS] = (value == BT_GATT_CCC_NOTIFY);
}

/*
 * Connection callback
 */
static void on_connected(struct bt_conn *conn, uint8_t err)
{
	if (err) {
		printk("Connection failed (err %u)\n", err);
		return;
	}

	printk("Connected\n");

	Gpio_LedWrite(BLE_CON_STATUS_LED, GPIO_ON);
}

/*
 * Cisconnection callback
 */
static void on_disconnected(struct bt_conn *conn, uint8_t reason)
{
	printk("Disconnected (reason %u)\n", reason);

	Gpio_LedWrite(BLE_CON_STATUS_LED, GPIO_OFF);
}


/*
 * Tasks definisions
 */
K_THREAD_DEFINE(BLE_IdThread, BLE_THREADS_STACKSIZE, BLE_Thread, NULL, NULL, NULL,
		BLE_THREAD_PRIO, 0, 0);
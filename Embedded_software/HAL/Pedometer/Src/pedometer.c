#include <zephyr/kernel.h>
#include "pedometer.h"
#include "lsm6dso.h"

#define PEDOMETER_THREADS_STACKSIZE (512U)
#define PEDOMETER_THREAD_PRIO (8U)


/*
 * Macros for data translation
 */
#define PEDOMETER_PROCESS_ACCELERATION_RAW_VAL(acc)      ((float)(acc) / 32768.0f * 4.0f)
#define PEDOMETER_PROCESS_ANGULAR_RATE_RAW_VAL(ang_rate) ((float)(ang_rate) / 32768.0f * 500.0f)
#define PEDOMETER_PROCESS_TEMPERATURE_RAW_VAL(temp)      ((float)(temp) / 256.0f + 25.0f)


Pedometer_Results_t Pedometer_Result = {0};

/*!	
 * \brief Task to handle pedometer data
 *
 * \param[in] None
 *
 * \retval None
 */
void Pedometer_Thread(void)
{
    LSM6DSO_Result_t result;

    while(1)
    {
        k_msleep(100);

        LSM6DSO_GetResults(&result);

        /* Process acceleration */
        Pedometer_Result.acceleration.x = PEDOMETER_PROCESS_ACCELERATION_RAW_VAL(result.acceleration.x);
        Pedometer_Result.acceleration.y = PEDOMETER_PROCESS_ACCELERATION_RAW_VAL(result.acceleration.y);
        Pedometer_Result.acceleration.z = PEDOMETER_PROCESS_ACCELERATION_RAW_VAL(result.acceleration.z);

        /* Process angular rate */
        Pedometer_Result.angular_rate.x = PEDOMETER_PROCESS_ANGULAR_RATE_RAW_VAL(result.angular_rate.x);
        Pedometer_Result.angular_rate.y = PEDOMETER_PROCESS_ANGULAR_RATE_RAW_VAL(result.angular_rate.y);
        Pedometer_Result.angular_rate.z = PEDOMETER_PROCESS_ANGULAR_RATE_RAW_VAL(result.angular_rate.z);

        /* Process temperature */
        Pedometer_Result.temperature = PEDOMETER_PROCESS_TEMPERATURE_RAW_VAL(result.temperature);

        /* Process steps */
        Pedometer_Result.steps = result.steps;

        // printk("Acc: x - %.2f y - %.2f z - %.2f \r\n", Pedometer_Result.acceleration.x, Pedometer_Result.acceleration.y, Pedometer_Result.acceleration.z);
        // printk("Ang: x - %.2f y - %.2f z - %.2f \r\n", Pedometer_Result.angular_rate.x, Pedometer_Result.angular_rate.y, Pedometer_Result.angular_rate.z);
        // printk("Temp: %.2f Steps: %d \r\n\n", Pedometer_Result.temperature, Pedometer_Result.steps);
    }
}

/*!	
 * \brief Pedometer results getter
 *
 * \param[in] results Results data
 * 
 * \retval None
 */
void Pedometer_GetResults(Pedometer_Results_t *results)
{
    results->acceleration.x = Pedometer_Result.acceleration.x;
    results->acceleration.y = Pedometer_Result.acceleration.y;
    results->acceleration.z = Pedometer_Result.acceleration.z;
    results->angular_rate.x = Pedometer_Result.angular_rate.x;
    results->angular_rate.y = Pedometer_Result.angular_rate.y;
    results->angular_rate.z = Pedometer_Result.angular_rate.z;
    results->temperature = Pedometer_Result.temperature;
    results->steps = Pedometer_Result.steps;
}







/*
 * Tasks definisions
 */
K_THREAD_DEFINE(Pedometer_IdThread, PEDOMETER_THREADS_STACKSIZE, Pedometer_Thread, NULL, NULL, NULL,
		PEDOMETER_THREAD_PRIO, 0, 0);
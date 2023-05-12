#include "lsm6dso_tasks.h"
#include "lsm6dso.h"
#include "lsm6dso_com.h"
#include "lsm6dso_regs.h"


#define LSM6DSO_THREADS_STACKSIZE (512U)

#define LSM6DSO_THREAD_INT1_PRIO (7U)
#define LSM6DSO_THREAD_INT2_PRIO (7U)

/*
 * Task to handle LSM6DSO intterrupt 1 (pedometer)
 */
void Lsm6dso_ThreadInt1(void)
{
    uint8_t status;

    while(1)
    {
        k_msleep(1000);

        /* Enable access to embedded functions registers */
        LSM6DSO_SingleWrite(LSM6DSO_REG_FUNC_CFG_ACCESS, LSM6DSO_FUNC_CFG_ACCESS);

        status = LSM6DSO_SingleRead(LSM6DSO_REG_EMB_FUNC_STATUS);

        if(status & LSM6DSO_IS_STEP_DET)
        {
            LSM6DSO_ReadSteps();
        }

        /* Disable access to embedded functions registers */
        LSM6DSO_SingleWrite(LSM6DSO_REG_FUNC_CFG_ACCESS, 0x00);

    }
}

/*
 * Task to handle LSM6DSO intterrupt 2 (accelerometer, gyro, temperature sensor)
 */
void Lsm6dso_ThreadInt2(void)
{
    uint8_t status;

    while(1)
    {
        k_msleep(1000);

        status = LSM6DSO_SingleRead(LSM6DSO_REG_STATUS_REG);

        while(0U != status)
        {
            if(status & LSM6DSO_XLDA)
            {
                LSM6DSO_ReadAcceleration();
            }

            if(status & LSM6DSO_GDA)
            {
                LSM6DSO_ReadAngularRate();
            }

            if(status & LSM6DSO_TDA)
            {
                LSM6DSO_ReadTemperature();
            }

            status = LSM6DSO_SingleRead(LSM6DSO_REG_STATUS_REG);
        }
    }
}

/*
 * Tasks definisions
 */
K_THREAD_DEFINE(Lsm6dso_IdThreadInt1, LSM6DSO_THREADS_STACKSIZE, Lsm6dso_ThreadInt1, NULL, NULL, NULL,
		LSM6DSO_THREAD_INT1_PRIO, 0, 0);
K_THREAD_DEFINE(Lsm6dso_IdThreadInt2, LSM6DSO_THREADS_STACKSIZE, Lsm6dso_ThreadInt2, NULL, NULL, NULL,
		LSM6DSO_THREAD_INT2_PRIO, 0, 0);

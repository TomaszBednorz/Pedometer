
#ifndef __LSM6DSO_TYPES_H_
#define __LSM6DSO_TYPES_H_

#include <stdint.h>

/*
 * Definitions of data types
 */

typedef struct 
{
    int16_t x;
    int16_t y;
    int16_t z;
}LSM6DSO_Acceleration_t;

typedef struct 
{
    int16_t x;
    int16_t y;
    int16_t z;
}LSM6DSO_AngularRate_t;

typedef struct 
{
    LSM6DSO_Acceleration_t acceleration; /* mg */
    LSM6DSO_AngularRate_t angular_rate;  /* dps */
    int16_t temperature;                 /* Cel deg */
    uint16_t steps;
}LSM6DSO_Result_t;

#endif
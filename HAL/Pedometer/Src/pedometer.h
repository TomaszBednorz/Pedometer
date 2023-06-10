#ifndef __PEDOMETER_H_
#define __PEDOMETER_H_

/*
 * Definitions of data types
 */

typedef struct 
{
    float x;
    float y;
    float z;
}Pedometer_Acceleration_t;

typedef struct 
{
    float x;
    float y;
    float z;
}Pedometer_AngularRate_t;

typedef struct 
{
    Pedometer_Acceleration_t acceleration; /* mg */
    Pedometer_AngularRate_t angular_rate;  /* dps */
    float temperature;                     /* Cel deg */
    uint16_t steps;
}Pedometer_Results_t;

/*
 * Functions prototypes
 */
void Pedometer_GetResults(Pedometer_Results_t *results);


#endif
#ifndef __LSM6DSO_H_
#define __LSM6DSO_H_

#include <stdint.h>
#include "lsm6dso_types.h"

/* 
 * Function prototypes
 */
void LSM6DSO_Init(void);
void LSM6DSO_GetResults(LSM6DSO_Result_t *results);
void LSM6DSO_ReadAcceleration(void);
void LSM6DSO_ReadAngularRate(void);
void LSM6DSO_ReadTemperature(void);
void LSM6DSO_ReadSteps(void);

#endif
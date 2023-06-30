#ifndef __LSM6DSO_COM_H_
#define __LSM6DSO_COM_H_

#include <stdint.h>

/* 
 * Function prototypes
 */
uint8_t LSM6DSO_SingleRead(uint8_t address);
void LSM6DSO_SingleWrite(uint8_t address, uint8_t data);
void LSM6DSO_MultipleRead(uint8_t address, uint8_t length, uint8_t* buf);
void LSM6DSO_MultipleWrite(uint8_t address, uint8_t length, uint8_t* buf);

#endif
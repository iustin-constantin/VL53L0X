/*
 * I2c.h
 *
 *  Created on: Apr 19, 2023
 *  Author: Constantin Iustin
 */

#ifndef I2C_H_
#define I2C_H_

#include <stdio.h>
#include "stm32f0xx.h"

uint8_t I2c_ucError;


void I2c_vInit(void);

void I2c_vStart(void);

void I2c_vStop(void);

// read N bytes
uint8_t I2c_vRead( uint8_t slave_address, uint8_t* data, uint8_t N );
// write N bytes
uint8_t I2c_vWrite( uint8_t slave_address, uint8_t* data, uint8_t N );

#endif /* I2C_H_ */

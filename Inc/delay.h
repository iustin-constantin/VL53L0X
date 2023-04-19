/*
 * delay.c
 *
 *  Created on: Apr 19, 2023
 *  Author: Constantin Iustin
 */

#ifndef INC_DELAY_H_
#define INC_DELAY_H_

#include <stdio.h>
#include "stm32f0xx.h"

#define DELAY_CPU_FREQUENCY             (32000000u)
/* Define number of miliseconds in one second */
#define DELAY_ONE_SEC_MS				(1000u)

volatile uint32_t ulDelay_timeMs;

void delay_vInit(void);
void delay_ms(uint32_t time_ms);


#endif /* INC_DELAY_H_ */

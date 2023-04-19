/*
 * delay.c
 *
 *  Created on: Apr 19, 2023
 *  Author: Constantin Iustin
 */
#include "delay.h"

void delay_vInit(void)
{
	SysTick_Config(32000000/1000); 	/*Set up a systick interrupt every millisecond */
	ulDelay_timeMs = 0u;
}

void delay_ms(uint32_t time_ms)
{
	ulDelay_timeMs = 0u;
	while(ulDelay_timeMs < time_ms);
}

void SysTick_Handler(void)  {                               /* SysTick interrupt Handler. */
	ulDelay_timeMs++;
}


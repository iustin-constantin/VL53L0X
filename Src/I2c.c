/*
 * I2c.c
 *
 *  Created on: Apr 19, 2023
 *  Author: Constantin Iustin
 */
#include"I2c.h"

void I2c_vInit(void)
{
	RCC->APB1ENR 	|= RCC_APB1ENR_I2C1EN;										/* Enable I2C clock 						*/
	RCC->AHBENR		|= RCC_AHBENR_GPIOBEN;										/* Enable GPIO clock						*/

	/* Configure I2C pins */
	GPIOB->MODER 	|= GPIO_MODER_MODER6_1 | GPIO_MODER_MODER7_1;				/* Set PB6 and PB7 as Alternate function 	*/
	GPIOB->OTYPER 	|= GPIO_OTYPER_OT_6 | GPIO_OTYPER_OT_7;						/* Set Open Drain for I2c pins PB6 and PB7 	*/
	GPIOB->AFR[0] 	|= (1 << GPIO_AFRL_AFSEL6_Pos)|(1 << GPIO_AFRL_AFSEL7_Pos); /* Set Alternate function for i2c pins		*/

	I2C1->CR1 		&= ~I2C_CR1_PE;												/* Disable peripheral I2C 					*/
	I2C1->TIMINGR 	|= 0x00201D2B;  											/* Set Prescaler, SCLH and SCLL to have a
	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 *	i2cclk of 100kHz						*/
	I2C1->ISR		|= I2C_ISR_TXE;
	I2C1->CR1 		|= I2C_CR1_PE;  											/* Enable peripheral I2C 					*/
}


void I2c_vStart(void)
{
	/* Send Start condition */
	I2C1->CR2 |=  (I2C_CR2_START);

//	/* wait for acknowledge */
	while (I2C1->CR2 & I2C_CR2_START);
}

void I2c_vStop(void)
{
	/* Send Stop condition */
	I2C1->CR2 |=  (I2C_CR2_STOP);

	/* wait for acknowledge */
	while (I2C1->CR2 & I2C_CR2_STOP);
	// Reset the ICR ('Interrupt Clear Register') event flag.
	I2C1->ICR |=  (I2C_ICR_STOPCF);
	while (I2C1->ICR & I2C_ICR_STOPCF);
}

/*	read N bytes from the I2C slave */
uint8_t I2c_vRead( uint8_t slave_address, uint8_t* data, uint8_t N )
{
	/* Set slave address. */
	I2C1->CR2   &= ~(I2C_CR2_SADD);
	I2C1->CR2   |=  (slave_address << 1u);
	/* Set R/W bit (1 for read)*/
	I2C1->CR2	|= 	I2C_CR2_RD_WRN;
	/* Set N bytes to read. */
	I2C1->CR2 &= ~(I2C_CR2_NBYTES);
	I2C1->CR2 |=  (N << I2C_CR2_NBYTES_Pos);

	I2c_vStart();

	do
	{
		/* Wait for a byte of data to be available, then read it.*/
		while (!(I2C1->ISR & I2C_ISR_RXNE));// || ((ulDelay_timeMs - t1) <= 10u));
		*data = (I2C1->RXDR & 0xFF);
		++data;
		N--;
	}while(N > 0u);

	I2c_vStop();
	return 0;
}

/* write N bytes to the I2C slave */
uint8_t I2c_vWrite( uint8_t slave_address, uint8_t* data, uint8_t N )
{
	/* Set slave address. */
	I2C1->CR2   &= ~(I2C_CR2_SADD);
	I2C1->CR2   |=  (slave_address << 1u);
	/* Set R/W bit (0 for write) */
	I2C1->CR2	&= 	~(I2C_CR2_RD_WRN);
	/* Set N bytes to write. */
	I2C1->CR2 &= ~(I2C_CR2_NBYTES);
	I2C1->CR2 |=  (N << I2C_CR2_NBYTES_Pos);

	I2c_vStart();

	do
	{
		/* write the byte to be sent into the data register */
		I2C1->TXDR = (I2C1->TXDR & 0xFFFFFF00) | (*data);
		/* Wait for one of these ISR bits:
		 * 'TXIS' ("ready for next byte")
		 * 'TC'   ("transfer complete") */
		while (!(I2C1->ISR & I2C_ISR_TXE));
		++data;
		N--;
	}while(N > 0u);
	I2c_vStop();
	return 0;
}

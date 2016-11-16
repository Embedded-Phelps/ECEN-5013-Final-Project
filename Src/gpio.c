/***************************************************************************
 *
 *	Filename: 		gpio.c
 *  Description:  	KL25Z GPIO functions implementation
 *  Author: 		ShuTing Guo  
 *  Date: 			Oct. 2016
 *	
 *  Pin Usage Info:
 *	PTB18	--	RED LED
 *	PTB19	--	GREEN LED
 *	PTD1	--	BLUE LED
 *	PTC5	--	SPI0 SCK
 *	PTC6	--	SPI0 MOSI
 *	PTC7	--	SPI0 MISO
 *	PTC12	--	SPI0 SS
 *	PTC13	--	NRF24L01 CE
 *	PTC16	--	NRF24L01 IRQ
 *	PTA1	--	UART0 RX
 *	PTA2	--	UART0 TX
 *	PTE24	--	I2C0 SCL
 *	PTE25	--	I2C0 SDA
 *	PTB16	--	TSI0_CH9
 *	PTB17	--	TSI0_CH10
 *	PTB0	--	ADC0_SE8
 
 *****************************************************************************/
 
#include "includes.h"

void board_Gpio_Init(void){
	
	/* Initialize pins for the RGB LEDs */
	PORTB_PCR18 |= (PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK);               // Red LED: GPIO, digital output 
    PORTB_PCR19 |= (PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK);               // GREENLED: GPIO, digital output
    PORTD_PCR1  |= (PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK);               // BLUE LED: GPIO, digital output
    FGPIOB_PSOR |= 1<<18;                                               // Port output set to logic 1 
    FGPIOB_PDDR |= 1<<18;                                               // Pin set for GPIO function 
    FGPIOB_PSOR |= 1<<19;                                               
    FGPIOB_PDDR |= 1<<19;                                               
    FGPIOD_PSOR |= 1<<1;                                            
    FGPIOD_PDDR |= 1<<1;                
	
	/* Initialize pins for the SPI0 interface */
	PORTC_PCR5 |= PORT_PCR_MUX(2);										// Set PTC5 pin to SPI0 SCK function	
	PORTC_PCR6 |= PORT_PCR_MUX(2);										// Set PTC6 pin to SPI0 MOSI function			
	PORTC_PCR7 |= PORT_PCR_MUX(2);										// Set PTC7 pin to SPI0 MISO function 	
	PORTC_PCR12|= PORT_PCR_MUX(1);										// Set PTC12 pin to GPIO function, serve as SS 
	FGPIOC_PSOR|= 1<<12;												// Set SS pin as Output, default logic high
	FGPIOC_PDDR|= 1<<12;                       

	/* Initialize pins for the NRF24L01 module */
	PORTC_PCR13|= PORT_PCR_MUX(1);										// Set PCR13 as a GPIO pin, serve as CE
	PORTC_PCR16|= PORT_PCR_MUX(1);										// Set PCR16 as a GPIO pin, serve as IRQ
	FGPIOC_PCOR|= 1<<13;												// CE as output, default low
	FGPIOC_PDDR|= 1<<13;												 
	FGPIOC_PDDR|= 0<<16;												// IRQ as input
	
	/* Initialize pins for the UART0 interface */
	PORTA_PCR1 = PORT_PCR_MUX(2);										// Set PTA1 pin to UART0 RX function
	PORTA_PCR2 = PORT_PCR_MUX(2);										// Set PTA2 pin to UART0 TX function
	
	/* Initialize pins for the I2C0 interface */
	PORTE_PCR24 = PORT_PCR_MUX(5);										// Set PTE24 pin to I2C0 SCL function
	PORTE_PCR25 = PORT_PCR_MUX(5);										// Set PTE25 pin to I2C0 SDA function
	
	/* Initialize pins for the TSI0 interface */
	PORTB_PCR16 = PORT_PCR_MUX(0);										// Set PTB16 pin to TSI0 CH9 function
	PORTB_PCR17 = PORT_PCR_MUX(0);										// Set PTB17 pin to TSI0 CH10 function
	
	/* Initialize pin for the Ultrasonic Range Finder input */
	PORTB_PCR0 = PORT_PCR_MUX(0);										// Set PTB0 pin to analog input function
}

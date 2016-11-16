/***************************************************************************
 *
 *	Filename: 		nRF24L01.h
 *  Description:  	nRF24L01 driver function prototype header file
 *  Author: 		ShuTing Guo  
 *  Date: 			Oct. 2016
 *
 *****************************************************************************/
#ifndef __NRF24L01_H
#define __NRF24L01_H

#ifdef __cplusplus
extern "C" {
#endif

#include "includes.h"

#define ADR_WIDTH	(5)			// 5 bytes TX(RX) address width
#define PLOAD_WIDTH	(32)			// 32 bytes TX(RX) payload

/******************************************************************
// nRF24L01 Register Map
*******************************************************************/
#define CONFIG			(0x00)		// Configuration Register
#define EN_AA			(0x01)
#define EN_RXADDR		(0x02)		// Enabled RX Addresses Register
#define SETUP_AW		(0x03)		// Setup of Address Widths Register
#define SETUP_RETR		(0x04)		// Setup of Automatic Retransmission Register
#define RF_CH			(0x05)		// RF Channel Register
#define RF_SETUP 		(0x06)		// RF Setup Register
#define STATUS			(0x07)		// Status Register
#define OBSERVE_TX		(0x08)		// Transmit Observe Register
#define CD				(0x09)		// Carrier Detect Register
#define RX_ADDR_P0		(0x0A)		// Receive Address Data Pipe 0 Register
#define RX_ADDR_P1		(0x0B)		// Receive Address Data Pipe 1 Register
#define RX_ADDR_P2		(0x0C)		// Receive Address Data Pipe 2 Register
#define RX_ADDR_P3		(0x0D)		// Receive Address Data Pipe 3 Register
#define RX_ADDR_P4		(0x0E)		// Receive Address Data Pipe 4 Register
#define RX_ADDR_P5		(0x0F)		// Receive Address Data Pipe 5 Register
#define TX_ADDR			(0x10)		// Transmit Address Register
#define RX_PW_P0		(0x11)
#define RX_PW_P1		(0x12)
#define RX_PW_P2		(0x13)
#define RX_PW_P3		(0x14)
#define RX_PW_P4		(0x15)
#define RX_PW_P5		(0x16)
#define FIFO_STATUS		(0x17)		// FIFO Status Register


#define RX_DR			(0x40)
#define TX_DS			(0x20)
#define MAX_RT			(0x01)
#define TX_FULL			(0x00)

#define SEND_FAILED		(0xFF)
#define RECEIVED_FAILED (0xFE)
#define NRF_UNDETECTED	(0xFD)
#define NRF_DETECTED	(0xFC)

/******************************************************************
// nRF24L01 SPI Command
*******************************************************************/
#define R_REGISTER		(0x00)
#define W_REGISTER		(0x20)
#define R_RX_PAYLOAD	(0x61)
#define W_TX_PAYLOAD	(0xA0)
#define FLUSH_TX		(0xE1)
#define FLUSH_RX		(0xE2)
#define REUSE_TX_PL		(0xE3)
#define ACTIVATE		(0x50)
#define R_RX_PL_WID		(0x60)
#define W_ACK_PAYLOAD	(0xA8)
#define W_TX_PAYLOAD_NOACK	(0xB0)
#define NOP				(0xFF)


#define NRF_ENABLE		(FGPIOC_PSOR |= (1<<13))
#define NRF_DISABLE		(FGPIOC_PCOR |= (1<<13))
#define NRF_IRQ			(FGPIOC_PDIR & (1<<16)) 

/******************************************************************
// nRF24L01 Control Functions
*******************************************************************/

/*********************************************************************
* @name: nrf_Init
*
* @description: Initialize the NRF24L01 module: configure pins,
*				initialize spi interface for communication 
*/
void nrf_Init(void);

/*********************************************************************
* @name: nrf_Write_Reg
*
* @description: write to a register of the NRF24L01
*               
* @param: reg -- target register
*		  value -- value to write to the target register
*         
* @return: value of the STATUS register
*/
uint8_t nrf_Write_Reg(uint8_t reg, uint8_t value);

/*********************************************************************
* @name: nrf_Read_Reg
*
* @description: read from a register of the NRF24L01
*               
* @param: reg -- target register to read from
*         
* @return: value of the register
*/
uint8_t nrf_Read_Reg(uint8_t reg);

/*********************************************************************
* @name: nrf_Write_Buf
*
* @description: write bytes of data to a register of the NRF24L01
*               
* @param: reg -- target register to write to
*         tx_Data_Buffer -- data to send
*		  bytes -- number of bytes 
*
* @return: value of the STATUS register
*/
uint8_t nrf_Write_Buf(uint8_t reg, uint8_t *tx_Data_Buffer, uint8_t bytes);

/*********************************************************************
* @name: nrf_Read_Buf
*
* @description: read bytes of data from a register of the NRF24L01
*               
* @param: reg -- target register to read from
*         rx_Data_Buffer -- storing read data
*		  bytes -- number of bytes 
*
* @return: value of the STATUS register
*/
uint8_t nrf_Read_Buf(uint8_t reg, uint8_t *rx_Data_Buffer, uint8_t bytes);

/*********************************************************************
* @name: nrf_Check
*
* @description: check if the NRF24L01 module is connected
*               
* @return: NRF_UNDETECTED / NRF_DETECTED
*/
uint8_t nrf_Check(void);

void nrf_RxMode(void);
void nrf_TxMode(void);
uint8_t nrf_TxPacket(uint8_t *data);
uint8_t nrf_RxPacket(uint8_t *data);

#ifdef __cplusplus
}
#endif


#endif

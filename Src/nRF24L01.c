#include "includes.h"

const uint8_t TX_ADDRESS[ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01}; //??
const uint8_t RX_ADDRESS[ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01}; //??

void nrf_Init(void){
	nrf_Gpio_Init();
	spi0_Init(MASTER);					// Initialize SPI interface for NRF24L01
}

uint8_t nrf_Write_Reg(uint8_t reg, uint8_t value){
	uint8_t status;
	SPI_SELECT;
	status = spi0_Sendbyte(reg);
	spi0_Sendbyte(value);
	SPI_DESELECT;
	return status;
}

uint8_t nrf_Read_Reg(uint8_t reg){
	uint8_t reg_Val,b;
	SPI_SELECT;
	b = spi0_Sendbyte(reg);
	reg_Val = spi0_Sendbyte(0xFF);
	SPI_DESELECT;
	return reg_Val;
}

uint8_t nrf_Write_Buf(uint8_t reg, uint8_t *tx_Buf, uint8_t bytes){
	volatile uint8_t status,gruff;
	uint8_t i;
	SPI_SELECT;
	status = spi0_Sendbyte(reg);
	for (i = 0; i < bytes; i++){
		gruff = spi0_Sendbyte(*tx_Buf++);
	}
	SPI_DESELECT;
	return status; 
}

uint8_t nrf_Read_Buf(uint8_t reg, uint8_t *rx_Buf, uint8_t bytes){
	uint8_t status;
	uint8_t i;
	SPI_SELECT;
	status = spi0_Sendbyte(reg);
	for (i = 0; i < bytes; i++)
		*rx_Buf++ = spi0_Sendbyte(0xFF);
	SPI_DESELECT;
	return status;
}

uint8_t nrf_Check(void){
	uint8_t buf[5]={0xA5,0xA5,0xA5,0xA5,0xA5};
	uint8_t i;
	//set spi speed?
	nrf_Write_Buf(W_REGISTER|TX_ADDR, buf, 5);
	nrf_Read_Buf(TX_ADDR, buf, 5);
	for (i=0; i<5; i++){
		if(buf[i]!=0xA5) break;
	}
	if(i!=5)
		return NRF_UNDETECTED;
	return NRF_DETECTED;
}

void nrf_TxMode(void){
	uint8_t test;
	NRF_DISABLE;
	nrf_Write_Buf(W_REGISTER|TX_ADDR, (uint8_t *)TX_ADDRESS, ADR_WIDTH);
	test = nrf_Read_Reg(STATUS);
	nrf_Write_Buf(W_REGISTER|RX_ADDR_P0, (uint8_t *)RX_ADDRESS, ADR_WIDTH);
	test = nrf_Read_Reg(STATUS);
	nrf_Write_Reg(W_REGISTER+EN_AA, 0x01);
	test = nrf_Read_Reg(STATUS);
	nrf_Write_Reg(W_REGISTER+EN_RXADDR, 0x01);
	test = nrf_Read_Reg(STATUS);
	nrf_Write_Reg(W_REGISTER+SETUP_RETR, 0x1A);
	test = nrf_Read_Reg(STATUS);
	nrf_Write_Reg(W_REGISTER+RF_CH, 40);
	test = nrf_Read_Reg(STATUS);
	nrf_Write_Reg(W_REGISTER+RF_SETUP, 0x0F);
	test = nrf_Read_Reg(STATUS);
	nrf_Write_Reg(W_REGISTER+CONFIG, 0x0E);
	test = nrf_Read_Reg(STATUS);
}

void nrf_RxMode(void){
	NRF_DISABLE;
	nrf_Write_Buf(W_REGISTER+RX_ADDR_P0, (uint8_t *)RX_ADDRESS, ADR_WIDTH);
	nrf_Write_Reg(W_REGISTER+EN_AA, 0x01);
	nrf_Write_Reg(W_REGISTER+EN_RXADDR, 0x01);
	nrf_Write_Reg(W_REGISTER+RF_CH, 40);
	nrf_Write_Reg(W_REGISTER+RX_PW_P0, PLOAD_WIDTH);
	nrf_Write_Reg(W_REGISTER+RF_SETUP, 0x0F);
	nrf_Write_Reg(W_REGISTER+CONFIG, 0x0F);
	NRF_ENABLE;
}

uint8_t nrf_TxPacket(uint8_t *tx_Buf){
	uint8_t status;
	uint32_t test;
	//set spi speed?
	status=nrf_Read_Reg(STATUS);
  nrf_Write_Buf(W_TX_PAYLOAD, tx_Buf, 3);
	test = NRF_IRQ;
	//NRF_ENABLE;
	while(NRF_IRQ);
	status = nrf_Read_Reg(STATUS);
	nrf_Write_Reg(W_REGISTER+STATUS, status);
  if(status & MAX_RT){
		nrf_Write_Reg(FLUSH_TX, 0xFF);
		status=nrf_Read_Reg(STATUS);
		return MAX_RT;
	}
	if(status & TX_DS){
		return TX_DS;
	}
	return SEND_FAILED;
}

uint8_t nrf_RxPacket(uint8_t *rx_Buf){
	uint8_t status;
	//set spi speed?
	status = nrf_Read_Reg(STATUS);
	nrf_Write_Reg(W_REGISTER+STATUS, status);
	if(status & RX_DR){
		nrf_Read_Buf(R_RX_PAYLOAD, rx_Buf, PLOAD_WIDTH);
		nrf_Write_Reg(FLUSH_RX, 0xFF);
		return RX_DR;
	}
	return RECEIVED_FAILED;
}

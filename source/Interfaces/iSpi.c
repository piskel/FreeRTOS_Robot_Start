/*
 * iSpi.c
 *
 *  Created on: 24 août 2021
 *      Author: luc.froideva
 */

#include "iDio.h"
#include "iSpi.h"
#include "MKL46Z4.h"

static bool isInit = false;

void iSpi_Setup()
	{
	if(isInit) return;
	// Enable SCGC4 for SPI0
	SIM->SCGC4 |= SIM_SCGC4_SPI0_MASK;

	iDio_EnablePortClk();

	PORTA->PCR[14] = PORT_PCR_MUX(2); // SPI0_PCS0
	PORTA->PCR[15] = PORT_PCR_MUX(2); // SPI0_SCK
	PORTA->PCR[16] = PORT_PCR_MUX(2); // SPI0_MOSI
	PORTA->PCR[17] = PORT_PCR_MUX(2); // SPI0_MISO

	// BaudRate Divisor = (SPPR+1) * 2(SPR+1)
	// BaudRate = SPI Clock Module (24Mhz) / BaudRate Divisor
	SPI0->BR &= ~SPI_BR_SPR_MASK;
	SPI0->BR &= ~SPI_BR_SPPR_MASK;
	SPI0->BR |= 0b00011000; // 1MHz

	SPI0->C1 |= SPI_C1_MSTR_MASK; // 1: Master mode
	SPI0->C1 &= ~SPI_C1_LSBFE_MASK; // 1: LSB, 0: MSB
	SPI0->C2 |= SPI_C2_SPIMODE_MASK; // 0: 8-bit transmission mode, 1: 16-bit transmission mode

	// HANDLES CHIP SELECT !!!!
	SPI0->C1 |= SPI_C1_SSOE_MASK;
	SPI0->C2 |= SPI_C2_MODFEN_MASK;

	// 4-Wire
	SPI0->C2 &= ~SPI_C2_SPC0_MASK;

	// Clock sampling
	SPI0->C1 &= ~SPI_C1_CPOL_MASK; // Sampling at rising edge
	SPI0->C1 &= ~SPI_C1_CPHA_MASK; // Clock resting state at low

	// No interruptions
	SPI0->C1 &= ~SPI_C1_SPIE_MASK;
	SPI0->C1 &= ~SPI_C1_SPTIE_MASK;
	isInit = true;
	}


void iSpi_Enable()
	{
	SPI0->C1 |= SPI_C1_SPE_MASK;
	}

void iSpi_Disable()
	{
	SPI0->C1 &= ~SPI_C1_SPE_MASK;
	}


bool iSpi_IsLastTickReceived()
	{
	return (SPI0->S & SPI_S_SPRF_MASK) == SPI_S_SPRF_MASK;
	}

bool iSpi_IsBufferEmpty()
	{
	return (SPI0->S & SPI_S_SPTEF_MASK) == SPI_S_SPTEF_MASK;
	}

bool iSpi_IsChipSelectLow()
	{
	return (SPI0->S & SPI_S_MODF_MASK) == SPI_S_MODF_MASK;
	}

char iSpi_SendData(UInt16 data)
	{
	// Waiting for buffer to empty
	while(!iSpi_IsBufferEmpty()){}

	SPI0->DH = (char)(data>>8);
	SPI0->DL = (char)(data&0x00FF);

	while(!iSpi_IsLastTickReceived()){}
	char DH = SPI0->DH;
	char DL = SPI0->DL;
	return DL;
	// TODO: Check if SPRF is 0
	}

UInt16 iSpi_ReadData(void)
	{
	while(!iSpi_IsBufferEmpty()){}
	SPI0->DH = 0;
	SPI0->DL = 0;

	while(!iSpi_IsLastTickReceived()){}

	return (SPI0->DH << 8) + SPI0->DL;
	}


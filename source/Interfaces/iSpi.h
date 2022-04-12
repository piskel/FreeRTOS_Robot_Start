/*
 * iSpi.h
 *
 *  Created on: 24 août 2021
 *      Author: luc.froideva
 */

#ifndef ISPI_H_
#define ISPI_H_

#include "def.h"

void iSpi_Setup();

void iSpi_Enable();
void iSpi_Disable();

bool iSpi_IsLastTickReceived();
bool iSpi_IsBufferEmpty();
bool iSpi_IsChipSelectLow();


char iSpi_SendData(UInt16 data);
UInt16 iSpi_ReadData(void);

#endif /* ISPI_H_ */

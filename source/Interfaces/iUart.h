/*
------------------------------------------------------------
Copyright 2003-201x Haute �cole ARC Ing�ni�rie, Switzerland. 
All rights reserved.
------------------------------------------------------------
File name : 	iUart.h	
Author and date :	Monnerat Serge 06.06.2014

Goal : interface du p�riph�rique RS232

-----------------------------------------------------------------------------
History:
-----------------------------------------------------------------------------

$History: $

-----------------------------------------------------------------------------
*/

#ifndef __iUart__
#define __iUart__

#include "def.h"

//------------------------------------------------------------
// Enum contenant le choix entre ENABLE et DISABLE de TX/RX
//------------------------------------------------------------
typedef enum
{
  kEn,
  kDis
}UartEnDisEnum;

//------------------------------------------------------------
// UART status flag
//------------------------------------------------------------
typedef enum
{
	kTransmitDataRegEmpty=0x80,
	kTransmitComplete=0x40,
	kReceiveDataRegFull=0x20,
	kIdleLineDetect=0x10,
	kOverrunErr=0x08,
	kNoiseErr=0x04,
	kFramingErr=0x02,
	kParityErr=0x01
}UartStatusEnum;

// Enum�ration des 2 ports UART
typedef enum
{
	kUart1,
	kUart2
}UartEnum;

//-----------------------------------------------------------------------
// UART 2 module configuration
//-----------------------------------------------------------------------
void iUart_Config(UartEnum aUart);

//------------------------------------------------------------
// Enable/Disable de la transmission --> TX
//------------------------------------------------------------
void iUart_EnDisTx(UartEnum aUart,UartEnDisEnum aEnDis);

//------------------------------------------------------------
// Enable/Disable de la r�ception
//------------------------------------------------------------
void iUart_EnDisRx(UartEnum aUart,UartEnDisEnum aEnDis);

//------------------------------------------------------------
// Get Uart2 flags state
// aStatus: which flag to read
// retour	: flag state
//------------------------------------------------------------
bool iUart_GetStatus(UartEnum aUart,UartStatusEnum aStatus);

//------------------------------------------------------------
// Uart2 Data register write
// aData: datas to transmit
//------------------------------------------------------------
void iUart_SetData(UartEnum aUart,unsigned char aData);

//------------------------------------------------------------
// Initialisation du buffer de r�ception
// !!! Il faut que l'interrupt soit disable --> probl�me de 
// concurence !!!
// aVal: la valeur avec laquelle on initialise le buffer 
//------------------------------------------------------------
void iUart_InitRxBuffer(UartEnum aUart,unsigned char aVal);

//------------------------------------------------------------
// Contr�le si le buffer est vide
// Return : true --> buffer empty, false --> buffer not empty
//------------------------------------------------------------
bool iUart_IsBufferEmpty(UartEnum aUart);

//------------------------------------------------------------
// Lecture d'un byte dans le buffer
// Return : retourne le plus vieux byte
//------------------------------------------------------------
Int8 iUart_GetCharFromBuffer(UartEnum aUart);

//------------------------------------------------------------
// Setup BaudRate
//------------------------------------------------------------
void iUart_SetupBaudrate(UartEnum aUart,UInt16 aBaud);


#endif

/*
------------------------------------------------------------
Copyright 2003-20xx Haute école ARC Ingéniérie, Switzerland. 
All rights reserved.
------------------------------------------------------------
Nom du fichier :	iUart.c
Auteur et Date :	Monnerat Serge 11.01.20xx

Description dans le fichier iUart.h
------------------------------------------------------------
*/

#include "iUart.h"
#include <MKL46Z4.h>
#include "mrs232.h"

// Fréquence du bus
#define kBusClockkHz 24000

// Vitesse de transmission
#define kUart2BaudRate 115200
#define kUart1BaudRate 38400

// Priorité de l'interruption RX
#define kUart2Prio 1
#define kUart1Prio 1

// 50 bytes buffer
#define kRxBufSize ((UInt8)(200)) 

// RX buffer control struct
static struct
{
	Int8     	RxBuf[kRxBufSize];
	UInt16 	  InIndex;
	UInt16 	  OutIndex;
	UInt16    ByteCount;
	bool      BufferIsFull;
}sUart1RxBufStruct,sUart2RxBufStruct;

//-----------------------------------------------------------------------
// UART 2 module configuration
//-----------------------------------------------------------------------
void iUart_Config(UartEnum aUart)
{
	unsigned int aSbr;
	
	if(aUart==kUart2)
		{
			// Enable du clock de l'UART2
			// System Clock Gating Control Register 4 (SIM_SCGC4)
			SIM->SCGC4|=SIM_SCGC4_UART2_MASK;
			
			// Configuration du crossbar pour les pin RX et TX (PTE16 et PTE17)
			// Pin Control Register n (PORTx_PCRn) --> UART = alternative 3
			PORTE->PCR[16]|=PORT_PCR_MUX(3);
			PORTE->PCR[17]|=PORT_PCR_MUX(3);
			
			// UART Control Register 1 (UARTx_C1)
			// PE = 0, pas de parité
			// M = 0 Normal --> start + 8 data bits (lsb first) + stop
			UART2->C1&= ~(UART_C1_M_MASK|UART_C1_PE_MASK );
			
			// Configuration de la vitesse de transmission
			// UART Baud Rate Registers:High (UARTx_BDH)
			// UART Baud Rate Registers: Low (UARTx_BDL)
			aSbr = (unsigned int)((kBusClockkHz*1000)/(kUart2BaudRate * 16));		
			UART2->BDH&=(~UART2->BDH);
			UART2->BDH|=UART_BDH_SBR(((aSbr & 0x1F00) >> 8));
			UART2->BDL= (unsigned char)(aSbr & UART_BDL_SBR_MASK);
			
			
			// Enable de l'interruption UART2 Rx au niveau du NVIC
			// Le vecteur du UART2 est le numéro 14
			NVIC_EnableIRQ(UART2_IRQn);
				
			// Configuration de la priorité de l'interruption PIT
			// O : plus haute priorité
			// 3 : plus basse priorité
			NVIC_SetPriority(UART2_IRQn, kUart2Prio);
			
			// Disable des interruptions
			// UART Control Register 2 (UARTx_C2)
			// UART Control Register 3 (UARTx_C3)
			UART2->C2&= (~(UART_C2_TIE_MASK|UART_C2_TCIE_MASK|UART_C2_RIE_MASK|UART_C2_ILIE_MASK));
			UART2->C3&= (~(UART_C3_ORIE_MASK|UART_C3_NEIE_MASK|UART_C3_FEIE_MASK|UART_C3_PEIE_MASK));
			
			// Enable de l'interruption RX
			// UART Control Register 2 (UARTx_C2)
			UART2->C2|=UART_C2_RIE_MASK;
		}
	else if(aUart==kUart1)
		{
			// Enable du clock de l'UART1
			// System Clock Gating Control Register 4 (SIM_SCGC4)
			SIM->SCGC4|=SIM_SCGC4_UART1_MASK;
			
			// Configuration du crossbar pour les pin RX et TX (PTE0 et PTE1)
			// Pin Control Register n (PORTx_PCRn) --> UART = alternative 3
			PORTE->PCR[0]|=PORT_PCR_MUX(3);//|PORT_PCR_PE_MASK|PORT_PCR_PS_MASK;
			PORTE->PCR[1]|=PORT_PCR_MUX(3);//|PORT_PCR_PE_MASK|PORT_PCR_PS_MASK;
			
			// UART Control Register 1 (UARTx_C1)
			// PE = 0, pas de parité
			// M = 0 Normal --> start + 8 data bits (lsb first) + stop
			UART1->C1&= ~(UART_C1_M_MASK|UART_C1_PE_MASK );
			
			// Configuration de la vitesse de transmission
			// UART Baud Rate Registers:High (UARTx_BDH)
			// UART Baud Rate Registers: Low (UARTx_BDL)
			aSbr = (unsigned int)((kBusClockkHz*1000)/(kUart1BaudRate * 16));		
			UART1->BDH&=(~UART1->BDH);
			UART1->BDH|=UART_BDH_SBR(((aSbr & 0x1F00) >> 8));
			UART1->BDL= (unsigned char)(aSbr & UART_BDL_SBR_MASK);
			
			
			// Enable de l'interruption UART2 Rx au niveau du NVIC
			// Le vecteur du UART2 est le numéro 13
			NVIC_EnableIRQ(UART1_IRQn);
				
			// Configuration de la priorité de l'interruption PIT
			// O : plus haute priorité
			// 3 : plus basse priorité
			NVIC_SetPriority(UART1_IRQn, kUart1Prio);
			
			// Disable des interruptions
			// UART Control Register 2 (UARTx_C2)
			// UART Control Register 3 (UARTx_C3)
			UART1->C2&= (~(UART_C2_TIE_MASK|UART_C2_TCIE_MASK|UART_C2_RIE_MASK|UART_C2_ILIE_MASK));
			UART1->C3&= (~(UART_C3_ORIE_MASK|UART_C3_NEIE_MASK|UART_C3_FEIE_MASK|UART_C3_PEIE_MASK));
			
			// Enable de l'interruption RX
			// UART Control Register 2 (UARTx_C2)
			UART1->C2|=UART_C2_RIE_MASK;
		}	
}

//------------------------------------------------------------
// Setup BaudRate
//------------------------------------------------------------
void iUart_SetupBaudrate(UartEnum aUart,UInt16 aBaud)
{
	unsigned int aSbr;
	
	if(aUart==kUart2)
		{
			// Configuration de la vitesse de transmission
			// UART Baud Rate Registers:High (UARTx_BDH)
			// UART Baud Rate Registers: Low (UARTx_BDL)
			aSbr = (unsigned int)((kBusClockkHz*1000)/(kUart2BaudRate * 16));		
			UART2->BDH&=(~UART2->BDH);
			UART2->BDH|=UART_BDH_SBR(((aSbr & 0x1F00) >> 8));
			UART2->BDL= (unsigned char)(aSbr & UART_BDL_SBR_MASK);
		}
	else if(aUart==kUart1)
		{
			// Configuration de la vitesse de transmission
			// UART Baud Rate Registers:High (UARTx_BDH)
			// UART Baud Rate Registers: Low (UARTx_BDL)
			aSbr = (unsigned int)((kBusClockkHz*1000)/(aBaud * 16));		
			UART1->BDH&=(~UART1->BDH);
			UART1->BDH|=UART_BDH_SBR(((aSbr & 0x1F00) >> 8));
			UART1->BDL= (unsigned char)(aSbr & UART_BDL_SBR_MASK);
		}
}

//------------------------------------------------------------
// Enable/Disable de la transmission --> TX
//------------------------------------------------------------
void iUart_EnDisTx(UartEnum aUart,UartEnDisEnum aEnDis)
{
	if(aUart==kUart2)
		{
			// UART Control Register 2 (UARTx_C2)
			if(kEn==aEnDis)
				{
					UART2->C2|=UART_C2_TE_MASK; 		// Enable TX
				}
			else if(kDis==aEnDis)
				{
					UART2->C2&=(~UART_C2_TE_MASK); // Disable TX
				}
		}
	else if(aUart==kUart1)
		{
			// UART Control Register 2 (UARTx_C2)
			if(kEn==aEnDis)
				{
					UART1->C2|=UART_C2_TE_MASK; 		// Enable TX
				}
			else if(kDis==aEnDis)
				{
					UART1->C2&=(~UART_C2_TE_MASK); // Disable TX
				}
		}
}


//------------------------------------------------------------
// Enable/Disable de la réception
//------------------------------------------------------------
void iUart_EnDisRx(UartEnum aUart,UartEnDisEnum aEnDis)
{
	if(aUart==kUart2)
		{
			// UART Control Register 2 (UARTx_C2)
			if(kEn==aEnDis)
				{
					UART2->C2|=UART_C2_RE_MASK; 		// Enable RX
				}
			else if(kDis==aEnDis)
				{
					UART2->C2&=(~UART_C2_RE_MASK); // Disable RX
				}
		}
	else if(aUart==kUart1)
		{
			// UART Control Register 2 (UARTx_C2)
			if(kEn==aEnDis)
				{
					UART1->C2|=UART_C2_RE_MASK; 		// Enable RX
				}
			else if(kDis==aEnDis)
				{
					UART1->C2&=(~UART_C2_RE_MASK); // Disable RX
				}
		}
}

//------------------------------------------------------------
// Get Uart flags state
// aStatus: which flag to read
// retour	: flag state
//------------------------------------------------------------
bool iUart_GetStatus(UartEnum aUart,UartStatusEnum aStatus)
{
	if(aUart==kUart2)
		{
			// UART Status Register 1 (UARTx_S1)
			return ((UART2->S1&aStatus)==aStatus);
		}
	else if(aUart==kUart1)
		{
			// UART Status Register 1 (UARTx_S1)
			return ((UART1->S1&aStatus)==aStatus);
		}
}

//------------------------------------------------------------
// Uart2 Data register write
// aData: datas to transmit
//------------------------------------------------------------
void iUart_SetData(UartEnum aUart,unsigned char aData)
{
	if(aUart==kUart2)
		{
			// UART Data Register (UARTx_D)
			UART2->D=aData;
		}
	else if(aUart==kUart1)
		{
			// UART Data Register (UARTx_D)
			UART1->D=aData;
		}
}

//------------------------------------------------------------
// Initialisation du buffer de réception
// !!! Il faut que l'interrupt soit disable --> problème de 
// concurence !!!
// aVal: la valeur avec laquelle on initialise le buffer 
//------------------------------------------------------------
void iUart_InitRxBuffer(UartEnum aUart,unsigned char aVal)
{
	UInt16 i=0;
	
	if(aUart==kUart2)
		{
			// Initialisation des index
			sUart2RxBufStruct.InIndex=0;
			sUart2RxBufStruct.OutIndex=0;
			
			// Reset du compteur de bytes
			sUart2RxBufStruct.ByteCount=0;
			
			// Reset du flag indiquant que le buffer est plein
			sUart2RxBufStruct.BufferIsFull=false;
			
			// Reset du buffer
			for(i=0;i<kRxBufSize;i++)
				{
					sUart2RxBufStruct.RxBuf[i]=aVal; 
				}
		}
	else if(aUart==kUart1)
		{
			// Initialisation des index
			sUart1RxBufStruct.InIndex=0;
			sUart1RxBufStruct.OutIndex=0;
			
			// Reset du compteur de bytes
			sUart1RxBufStruct.ByteCount=0;
			
			// Reset du flag indiquant que le buffer est plein
			sUart1RxBufStruct.BufferIsFull=false;
			
			// Reset du buffer
			for(i=0;i<kRxBufSize;i++)
				{
					sUart1RxBufStruct.RxBuf[i]=aVal; 
				}
		}
	
}

//------------------------------------------------------------
// Contrôle si le buffer est vide
// Return : true --> buffer empty, false --> buffer not empty
//------------------------------------------------------------
bool iUart_IsBufferEmpty(UartEnum aUart)
{
	bool aRet=false;
	
	if(aUart==kUart2)
		{
			if(sUart2RxBufStruct.ByteCount==0)
				{
					aRet=true;
				}
			else
				{
					aRet=false;
				}
		}
	else if(aUart==kUart1)
		{
			if(sUart1RxBufStruct.ByteCount==0)
				{
					aRet=true;
				}
			else
				{
					aRet=false;
				}
		}

	return aRet;
}

//------------------------------------------------------------
// Lecture d'un byte dans le buffer
// Return : retourne le plus vieux byte
//------------------------------------------------------------
Int8 iUart_GetCharFromBuffer(UartEnum aUart)
{
  Int8 aChar=0;
	
  if(aUart==kUart2)
  	{
			// Byte read
			aChar=sUart2RxBufStruct.RxBuf[sUart2RxBufStruct.OutIndex];
			
			// Incrémentation de l'index
			sUart2RxBufStruct.OutIndex++;
			
			// Buffer tournant
			if(sUart2RxBufStruct.OutIndex>=kRxBufSize)
				{
					sUart2RxBufStruct.OutIndex=0;
				}
			
			// Décrémentation du compteur de bytes
			sUart2RxBufStruct.ByteCount--;
  	}
  else if(aUart==kUart1)
  	{
			// Byte read
			aChar=sUart1RxBufStruct.RxBuf[sUart1RxBufStruct.OutIndex];
			
			// Incrémentation de l'index
			sUart1RxBufStruct.OutIndex++;
			
			// Buffer tournant
			if(sUart1RxBufStruct.OutIndex>=kRxBufSize)
				{
					sUart1RxBufStruct.OutIndex=0;
				}
			
			// Décrémentation du compteur de bytes
			sUart1RxBufStruct.ByteCount--;
  	}
	 
	// Retourne un byte du buffer
	return aChar;
}

//---------------------------------------------------------------------------
// RX interrupt 
//---------------------------------------------------------------------------
void UART1_IRQHandler(void)
{
	Int8 aVal;
	static UInt8 aRxStatus;
	
	// Lecture du registre de statut pour effacer RDRF flag
	aRxStatus=UART1->S1;
	aRxStatus=aRxStatus;	// Pour le warning du compilateur
	
	// Lecture de la donnée reçue
	aVal=UART1->D;
	
	// On ressort sur le port série 2 les réponses du module
	// Attente fin d'envoi
	while(!iUart_GetStatus(kUart2,kTransmitComplete));
	// Ecriture du registre de données
	iUart_SetData(kUart2,aVal);
	
	if(sUart1RxBufStruct.ByteCount>=kRxBufSize)
	  {
	    // Buffer full flag
	    sUart1RxBufStruct.BufferIsFull=true;
	  }
	else
	  {
	    // Buffer not full flag
	    sUart1RxBufStruct.BufferIsFull=false;
	    
	    // Sauvegarde de la donnée reçue
	    sUart1RxBufStruct.RxBuf[sUart1RxBufStruct.InIndex]=aVal;
	    
	    // Incrémentation de l'index
	    sUart1RxBufStruct.InIndex++;
	    
	    // Incrémentation du compteur de byte
	    sUart1RxBufStruct.ByteCount++;
	
	    // Buffer tournant
	    if(sUart1RxBufStruct.InIndex>=kRxBufSize)
	      {
	        sUart1RxBufStruct.InIndex=0;
	      }
	  }
}

//---------------------------------------------------------------------------
// RX interrupt 
//---------------------------------------------------------------------------
void UART2_IRQHandler(void)
{
	Int8 aVal;
	static UInt8 aRxStatus;
	
	// Lecture du registre de statut pour effacer RDRF flag
	aRxStatus=UART2->S1;
	aRxStatus=aRxStatus;	// Pour le warning du compilateur
	
	// Lecture de la donnée reçue
	aVal=UART2->D;
	
	
	if(sUart2RxBufStruct.ByteCount>=kRxBufSize)
	  {
	    // Buffer full flag
	    sUart2RxBufStruct.BufferIsFull=true;
	  }
	else
	  {
	    // Buffer not full flag
	    sUart2RxBufStruct.BufferIsFull=false;
	    
	    // Sauvegarde de la donnée reçue
	    sUart2RxBufStruct.RxBuf[sUart2RxBufStruct.InIndex]=aVal;
	    
	    // Incrémentation de l'index
	    sUart2RxBufStruct.InIndex++;
	    
	    // Incrémentation du compteur de byte
	    sUart2RxBufStruct.ByteCount++;
	
	    // Buffer tournant
	    if(sUart2RxBufStruct.InIndex>=kRxBufSize)
	      {
	        sUart2RxBufStruct.InIndex=0;
	      }
	  }
}



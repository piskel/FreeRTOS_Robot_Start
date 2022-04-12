/*
------------------------------------------------------------
Copyright 2003-20xx Haute �cole ARC Ing�ni�rie, Switzerland. 
All rights reserved.
------------------------------------------------------------
Nom du fichier :	mLine.c
Auteur et Date :	Monnerat Serge 11.01.20xx

Description dans le fichier mLine.h
------------------------------------------------------------
*/

#include "mLine.h"
#include "iI2c.h"


// Adresse du chip SX1509 permettant de lire par I2C l'�tat des photodiodes
#define kLineWriteAdr 	0x7C
#define kLineReadAdr 	0x7D

// Prototype des fonctions statiques
static bool mLine_GetData(UInt8 aReg, UInt8* aData);
static bool mLine_SetData(UInt8 aReg,UInt8 aVal);

//-----------------------------------------------------------------------------
// Configuration du module      
//-----------------------------------------------------------------------------
void mLine_Setup(void)
{	
  // Configure le diviseur du clock
	iI2C_Config();
}

//-----------------------------------------------------------------------------
// Ouverture du module      
//-----------------------------------------------------------------------------
void mLine_Open(void)
{
	bool  aRet;
	UInt8 aDataH;
	UInt8 aDataL;
	
	// Enable IIC
  iI2C_Enable();
  
  // Lecture de l'ID du capteur --> test
  
  // Reset du chip SX1509
  aRet=mLine_SetData(REG_RESET,0x12);
  aRet=mLine_SetData(REG_RESET,0x34);
  // This should return 0xFF00
  aRet=mLine_GetData(REG_INTERRUPT_MASK_A, &aDataH);
  aRet=mLine_GetData(REG_SENSE_HIGH_B, &aDataL);

  // Config du chip SX1509
  // Port A en sortie
  // Port B bit 0 et 1 en entr�e
  aRet=mLine_SetData(REG_DIR_A,0xFF);
  aRet=mLine_SetData(REG_DIR_B,0xFC);
  aRet=mLine_SetData(REG_DATA_B,0x00);
}

//-----------------------------------------------------------------------------
// Fermeture du module      
//-----------------------------------------------------------------------------
void mLine_Close(void)
{
  // Disable IIC
  iI2C_Disable();	
}

//-----------------------------------------------------------------------------
// Lecture d'un registre du chip SX1509 permettant de lire par I2C l'�tat des photodiodes
// aReg:    le registre que l'on veut lire
// aData:   l'adresse du variable dans laquelle on �crit le contenu du registre
// retour : true --> lecture OK, false --> lecture KO
//-----------------------------------------------------------------------------
static bool mLine_GetData(UInt8 aReg, UInt8* aData)
{
	bool aNoAck=false;
		
	// Disable transmit ACK
  iI2C_SetAckMode(kNoAck);

  // Attend que le bus soit libre
  while(true==iI2C_ReadStatus(kBUSY)); 
	
	//-----------------------------------------------------------------------------
	// D'abords en WRITE afin de transmettre le registre 
	// que l'on veut lire ainsi que l'adresse du slave
	//-----------------------------------------------------------------------------
		
	// D�but de la transmission --> mode write et START condition 
  iI2C_TxRxSelect(kTxMode); 
  iI2C_SetStartState();
  
  // Transmission de l'adresse en WRITE du slave dans le registre de donn�es
	// --> obligatoire protocolle I2C, le slave doit d'abords r�pondre � son adresse
	iI2C_SendData(kLineWriteAdr);

	// Attend la fin de la transmission
	iI2C_WaitEndOfRxOrTx();
	
	// Est-ce que le slave est content --> Read Ack
	aNoAck=iI2C_ReadStatus(kRxAK);
	
	if(aNoAck==true)
		{
			// FIN de la lecture
			iI2C_TxRxSelect(kTxMode);
			iI2C_SetAckMode(kNoAck);
			iI2C_SetStopState();
			return false;
		}
	
	// Transmission de l'adresse du registre que l'on veut lire
	iI2C_SendData(aReg);
	
	// Attend la fin de la transmission
	iI2C_WaitEndOfRxOrTx();
	
	// Est-ce que le slave est content --> Read Ack
	aNoAck=iI2C_ReadStatus(kRxAK);
	
	if(aNoAck==true)
		{
			// FIN de la lecture
			iI2C_TxRxSelect(kTxMode);
			iI2C_SetAckMode(kNoAck);
			iI2C_SetStopState();
			return false;
		}
			
	// Nouvelle condition START 
	iI2C_SetRepeatedStartSate();
	
	//-----------------------------------------------------------------------------
	// Passage en READ
	//-----------------------------------------------------------------------------
	
	// Transmission de l'adresse en READ du slave dans le registre de donn�es
	// --> obligatoire protocolle I2C, le slave doit d'abords r�pondre � son adresse
	iI2C_SendData(kLineReadAdr);
	
	// Attend la fin de la transmission
	iI2C_WaitEndOfRxOrTx();
	
	// Est-ce que le slave est content --> Read Ack
	aNoAck=iI2C_ReadStatus(kRxAK);
	
	if(aNoAck==true)
		{
			// FIN de la lecture
			iI2C_TxRxSelect(kTxMode);
			iI2C_SetAckMode(kNoAck);
			iI2C_SetStopState();
			return false;
		}
	 
	// Passage en mode READ 
	// --> En lecture c'est au master d'envoyer le ACK mais comme on 
	// lit q'un byte pas besoin de ACK
	//iI2C_EnableTxAck();
	iI2C_TxRxSelect(kRxMode); 
	
	// Lecture qui ne sert � rien --> permet l'�mission de l'horloge n�cessaire au
	// slave afin de transmettre sa donn�e
	iI2C_ReadData();
	
	// Attend la fin de la transmission
	iI2C_WaitEndOfRxOrTx();
	 
	// FIN de la lecture
	// G�n�ration de la condition STOP
	// Obligatoire de faire le STOP avant la lecture pour 
	// ne pas �mettre de clock � nouveau!
	iI2C_TxRxSelect(kTxMode);
	iI2C_SetAckMode(kNoAck);
	iI2C_SetStopState();
	
	// Lecture de la valeur du registre demand�
	*aData=iI2C_ReadData();
	
	return true;	
}

//-----------------------------------------------------------------------------
// Ecriture d'un registre du chip SX1509 permettant de lire par I2C l'�tat des photodiodes
// aReg:    le registre que l'on veut �crire
// aData:   le contenu du registre
// retour : true --> lecture OK, false --> lecture KO
//-----------------------------------------------------------------------------
static bool mLine_SetData(UInt8 aReg,UInt8 aVal)
{
	bool aNoAck=false;
	
	// Disable transmit ACK
  iI2C_SetAckMode(kNoAck);

  // Attend que le bus soit libre
  while(true==iI2C_ReadStatus(kBUSY)); 
	
	//-----------------------------------------------------------------------------
	// D'abords en WRITE afin de transmettre le registre 
	// que l'on veut lire ainsi que l'adresse du slave
	//-----------------------------------------------------------------------------
		
	// D�but de la transmission --> mode write et START condition 
  iI2C_TxRxSelect(kTxMode); 
  iI2C_SetStartState();
  
  // Transmission de l'adresse en WRITE du slave dans le registre de donn�es
	// --> obligatoire protocolle I2C, le slave doit d'abords r�pondre � son adresse
	iI2C_SendData(kLineWriteAdr);
	
	// Attend la fin de la transmission
	iI2C_WaitEndOfRxOrTx();
	
	// Est-ce que le slave est content --> Read Ack
	aNoAck=iI2C_ReadStatus(kRxAK);
	
	if(aNoAck==true)
		{
			// FIN de la lecture
			iI2C_TxRxSelect(kTxMode);
			iI2C_SetAckMode(kNoAck);
			iI2C_SetStopState();
			return false;
		}
	
	
	// Transmission de l'adresse du registre que l'on veut �crire
	iI2C_SendData(aReg);
		
	// Attend la fin de la transmission
	iI2C_WaitEndOfRxOrTx();
	
	// Est-ce que le slave est content --> Read Ack
	aNoAck=iI2C_ReadStatus(kRxAK);
	
	if(aNoAck==true)
		{
			// FIN de la lecture
			iI2C_TxRxSelect(kTxMode);
			iI2C_SetAckMode(kNoAck);
			iI2C_SetStopState();
			return false;
		}


	// Transmission de la donn�e que l'on veut �crire dans le registre
	iI2C_SendData(aVal);
		
	// Attend la fin de la transmission
	iI2C_WaitEndOfRxOrTx();
	
	// Est-ce que le slave est content --> Read Ack
	aNoAck=iI2C_ReadStatus(kRxAK);
	
	// FIN de la lecture
	iI2C_TxRxSelect(kTxMode);
	iI2C_SetAckMode(kNoAck);
	iI2C_SetStopState();
	if(aNoAck==true)
		{
			return false;
		}
	else
		{
			return true;
		}		
}

//-----------------------------------------------------------------------------
// Lecture des 8 "pixel" du chip SX1509 permettant de lire par I2C l'�tat des photodiodes
// *aPixel: 	adresse de la variable contenant les 8 pixels (0=noir, 1=blanc)
// retour : true --> lecture OK, false --> lecture KO
//-----------------------------------------------------------------------------
bool mLine_GetRaw(UInt8 *aPixel)
{
	bool aRet;

	aRet=mLine_GetData(REG_DATA_A, aPixel);
	*aPixel=~(*aPixel);

	return aRet;
}




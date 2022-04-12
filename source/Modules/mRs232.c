/*
------------------------------------------------------------
Copyright 2003-20xx Haute école ARC Ingéniérie, Switzerland. 
All rights reserved.
------------------------------------------------------------
Nom du fichier :	mRs232.c
Auteur et Date :	Monnerat Serge 11.01.20xx

Description dans le fichier mRs232.h
------------------------------------------------------------
*/

#include "iUart.h"
#include "mRs232.h"

//-------------------------------------------------------------------------
// Configuration du module SCI   
//-------------------------------------------------------------------------
void mRs232_Setup(void)
{
  //------------------------------------------------------------
  // Configuration du port RS232
  //------------------------------------------------------------
  iUart_Config(kUart2);
  
  // Init du buffer de réception avec 0
  iUart_InitRxBuffer(kUart2,0);
}

//-------------------------------------------------------------------------
// Ouverture des interfaces    
//-------------------------------------------------------------------------
void mRs232_Open(void)
{
	// Enable de la transmission --> TX
  iUart_EnDisTx(kUart2,kEn);
  
  // Enable de la réception --> RX
  iUart_EnDisRx(kUart2,kEn);
}

//-------------------------------------------------------------------------
// Envoi d'un caractère par RS232
// aChar	: la caractère à transmettre
//-------------------------------------------------------------------------
void mRs232_WriteChar(Int8 aChar)
{
	// Attente fin d'envoi
	while(!iUart_GetStatus(kUart2,kTransmitComplete));
	
	// Ecriture du registre de données
	iUart_SetData(kUart2,aChar);
}

//-------------------------------------------------------------------------
// Envoi d'une chaîne de caractères par RS232
// La tramsmission s'arrête au caractère NULL
// *aDataPtr	: pointeur sur la chaîne de caractères à transmettre, la chaîne
//              doit-être terminée par le caractère NULL("ma chaîne")
//-------------------------------------------------------------------------
void mRs232_WriteString(Int8 *aDataPtr)
{
	// Tant que toutes les données ne sont pas émises
	while(*aDataPtr!=0)
		{
			// Attente fin d'envoi
			while(!iUart_GetStatus(kUart2,kTransmitComplete));
	
			// Ecriture du registre de données
			iUart_SetData(kUart2,*aDataPtr);
			
			// prochain byte du buffer
			aDataPtr++;
		}
}

//-----------------------------------------------------------------------------
// Lecture d'un byte du buffer de réception. Le buffer est rempli par 
// l'interruption Receive data reg full
// *aBytePtr	  : pointeur sur la variable recevant la donnée
// Retour 	    : true --> le buffer est vide, false le buffer est pas vide
//-----------------------------------------------------------------------------
bool mRs232_ReadDataFromBuffer(UInt8 *aBytePtr)
{
  bool aRet=false;
  	
	// Contrôle si le buffer est vide
  aRet=iUart_IsBufferEmpty(kUart2);
	
	// Si le buffer n'est pas vide --> lecture d'un byte
	if(false==aRet)
	  {
	    // Lecture d'un byte du buffer de réception
      *aBytePtr=iUart_GetCharFromBuffer(kUart2);
	  }

	return aRet;
}

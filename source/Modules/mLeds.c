/*
------------------------------------------------------------
Copyright 2003-20xx Haute �cole ARC Ing�ni�rie, Switzerland. 
All rights reserved.
------------------------------------------------------------
Nom du fichier :	mLeds.c
Auteur et Date :	Monnerat Serge 11.01.20xx

Description dans le fichier mLeds.h
------------------------------------------------------------
*/

#include "mLeds.h"
#include "iDio.h"


//-----------------------------------------------------------------------------
// Configuration des ports IO utilis�s par les LEDS
//-----------------------------------------------------------------------------
void mLeds_Setup(void)
{
		// Enable du clock des GPIO
		iDio_EnablePortClk();
		
		// Configuration de la fonctionnalit� de chaque PIN
		iDio_PinConfig(kPortC,kPin8,kAlternate1);
		iDio_PinConfig(kPortC,kPin9,kAlternate1);
		iDio_PinConfig(kPortC,kPin10,kAlternate1);
		iDio_PinConfig(kPortC,kPin11,kAlternate1);
		
		// Configuration des Ports en entr�e ou en sortie
	  iDio_SetPortDirection(kPortC,kMaskIo8+kMaskIo9+kMaskIo10+kMaskIo11,kIoOutput);
}

//-----------------------------------------------------------------------------
// On Off des LED
// aMask:   choix des LED sur lesquelles on veut agir (si bit � 1)
// aState:  choix de l'�tat des LED d�sir�s
//-----------------------------------------------------------------------------
void mLeds_Write(LedMaskEnum aMask,LedStateEnum aState)
{  
  unsigned long aTmp;
    
	// Commande des LED --> PORTC
  if(kLedOn==aState)
    {
  		iDio_SetPort(kPortC,aMask,kIoOn);
    }
  else if(kLedOff==aState)
    {
			iDio_SetPort(kPortC,aMask,kIoOff);
    }
}

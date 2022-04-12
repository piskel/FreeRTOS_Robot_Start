/*
------------------------------------------------------------
Copyright 2003-201x Haute école ARC Ingéniérie, Switzerland.
All rights reserved.
------------------------------------------------------------
File name : 	mLeds.h
Author and date :	Monnerat Serge 06.06.20xx

Goal : outils afin de pouvoir exploiter les LEDS

-----------------------------------------------------------------------------
History:
-----------------------------------------------------------------------------

$History: $

-----------------------------------------------------------------------------
*/

#ifndef MLEDS_H_
#define MLEDS_H_

#include"def.h"

// Masque des LED
typedef enum
{
  kMaskLed1=0x100,
  kMaskLed2=0x200,
  kMaskLed3=0x400,
  kMaskLed4=0x800,
  kMaskLedAll=0xF00
}LedMaskEnum;

// Etat des LED
typedef enum
{
  kLedOff,
  kLedOn
}LedStateEnum;

//-----------------------------------------------------------------------------
// Configuration des ports IO utilisés par les LEDS
//-----------------------------------------------------------------------------
void mLeds_Setup(void);

//-----------------------------------------------------------------------------
// On Off des LED
// aMask:   choix des LED sur lesquelles on veut agir (si bit à 1)
// aState:  choix de l'état des LED désirés
//-----------------------------------------------------------------------------
void mLeds_Write(LedMaskEnum aMask,LedStateEnum aState);

#endif /* MLEDS_H_ */

/*
------------------------------------------------------------
Copyright 2003-201x Haute école ARC Ingéniérie, Switzerland.
All rights reserved.
------------------------------------------------------------
File name : 	mSwitch.h
Author and date :	Monnerat Serge 06.06.20xx

Goal : outils afin de pouvoir exploiter le interrupteurs et poussoirs

-----------------------------------------------------------------------------
History:
-----------------------------------------------------------------------------

$History: $

-----------------------------------------------------------------------------
*/

#ifndef MSWITCH_H_
#define MSWITCH_H_

#include"def.h"

// Enuméré des interrupteurs (DIL_SW_0 à DIL_SW_7 sur le schéma)
typedef enum
{
	kSw0=0,
	kSw1=1,
	kSw2=2,
	kSw3=3
}SwitchEnum;

// Enuméré des boutons poussoirs (SW_PUSH_0 à SW_PUSH_3  sur le schéma)
typedef enum
{
	kPushButSW0=0,
	kPushButSW1=1,
	kPushButSW2=2,
	kPushButSW3=3
}PushButEnum;

//-----------------------------------------------------------------------------
// Configuration des ports IO utilisés par les interrupteurs et les poussoirs
//-----------------------------------------------------------------------------
void mSwitch_Setup(void);

//-------------------------------------------------------------------------
// Lecture des interrupteurs
// aSwitchNb: quel interrupteur veut-on lire
// Retour: état du switch
//-------------------------------------------------------------------------
bool mSwitch_ReadSwitch(SwitchEnum aSwitch);

//-------------------------------------------------------------------------
// Lecture des boutons poussoir
// aPushButNb: quel boutons poussoir veut-on lire
// Retour: état du bouton poussoir
//-------------------------------------------------------------------------
bool mSwitch_ReadPushBut(PushButEnum aPushButNb);

#endif /* MSWITCH_H_ */

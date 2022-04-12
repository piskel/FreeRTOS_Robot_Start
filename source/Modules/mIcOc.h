/*
------------------------------------------------------------
Copyright 2003-20xx Haute école ARC Ingéniérie, Switzerland. 
All rights reserved.
------------------------------------------------------------
Nom du fichier : 	mIcOc.h	
Auteur et Date :	Monnerat Serge 11.01.20xx

But : Module permettant l'exploitation des fonctions Input Capture et
			Output Compare

Modifications
Date		Faite	Ctrl		Description
------------------------------------------------------------
*/

#ifndef __MICOC__
#define __MICOC__

#include "def.h"

// Sens des moteurs
typedef enum
{
	kEnAvant,
	kEnArriere
}SensEnum;

//-----------------------------------------------------------------------------
// Configuration du module TIMER
//-----------------------------------------------------------------------------
void mIcOc_Setup(void);

//-----------------------------------------------------------------------------
// Ouverture de l'interface    
//-----------------------------------------------------------------------------
void mIcOc_Open(void);

//-----------------------------------------------------------------------------
// Fermeture de l'interface   
//-----------------------------------------------------------------------------
void mIcOc_Close(void);

//------------------------------------------------------------
// Lecture de la distance en cm
//------------------------------------------------------------
float mIcOc_GetDistFront(void);

//------------------------------------------------------------
// Lecture de la distance en cm
//------------------------------------------------------------
float mIcOc_GetDistBack(void);

//------------------------------------------------------------
// Lecture de la distance en cm
//------------------------------------------------------------
float mIcOc_GetDistLeft(void);

//------------------------------------------------------------
// Lecture de la distance en cm
//------------------------------------------------------------
float mIcOc_GetDistRight(void);

//-----------------------------------------------------------------------------
// Moteur gauche en avant ou en arrière
// aDuty: pwm duty cycle (0.0 to 1.0 --> 0% to 100%)
// aSens: en avant ou en arrière
//-----------------------------------------------------------------------------
void mIcOc_MoteurGauche(SensEnum aSens,float aDuty);

//-----------------------------------------------------------------------------
// Moteur droit en avant ou en arrière
// aDuty: pwm duty cycle (0.0 to 1.0 --> 0% to 100%)
// aSens: en avant ou en arrière
//-----------------------------------------------------------------------------
void mIcOc_MoteurDroit(SensEnum aSens,float aDuty);

//------------------------------------------------------------
// Démarre une mesure de distance
//------------------------------------------------------------
void mIcOc_StartDistMesure(void);


#endif

/*
------------------------------------------------------------
Copyright 2003-2010 Haute école ARC Ingéniérie, Switzerland. 
All rights reserved.
------------------------------------------------------------
File name : 	iTpm.h	
Author and date :	Monnerat Serge 1 mars 2012

Goal : Interface des fonctions Input Capture et Output Compare du Timer

-----------------------------------------------------------------------------
History:
-----------------------------------------------------------------------------

$History: $


-----------------------------------------------------------------------------
*/

#ifndef __ITPM__
#define __ITPM__

#include "def.h"

// Pwm channels enum
typedef enum
{
	kPwmCh1,
	kPwmCh2,
	kPwmCh3,
	kPwmCh4
}PwmChannelEnum;

//------------------------------------------------------------
// TPM setup
//------------------------------------------------------------
void iTPM_Config(void);

//------------------------------------------------------------
// Set PWM duty
//------------------------------------------------------------
void iTPM_UpdatePWM(PwmChannelEnum aChannel, float aDuty);

//------------------------------------------------------------
// Lecture de la distance en cm
//------------------------------------------------------------
float iTPM_GetDistFront(void);

//------------------------------------------------------------
// Lecture de la distance en cm
//------------------------------------------------------------
float iTPM_GetDistBack(void);

//------------------------------------------------------------
// Lecture de la distance en cm
//------------------------------------------------------------
float iTPM_GetDistLeft(void);

//------------------------------------------------------------
// Lecture de la distance en cm
//------------------------------------------------------------
float iTPM_GetDistRight(void);

#endif

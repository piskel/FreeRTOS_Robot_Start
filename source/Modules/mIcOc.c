/*
------------------------------------------------------------
Copyright 2003-20xx Haute école ARC Ingéniérie, Switzerland. 
All rights reserved.
------------------------------------------------------------
Nom du fichier :	mIcOc.c
Auteur et Date :	Monnerat Serge 11.01.20xx

Description dans le fichier mIcOc.h
------------------------------------------------------------
*/

#include "mIcOc.h"
#include "iTPM.h"
#include "iDio.h"
#include "FreeRTOS.h"
#include "task.h"

//-----------------------------------------------------------------------------
// Configuration du module TIMER
//-----------------------------------------------------------------------------
void mIcOc_Setup(void)
{
	UInt16 i;
	
	// Configuration du TIMER
	iTPM_Config();
	
	// Trig des capteurs MaxSonar
	iDio_PinConfig(kPortC,kPin16,kAlternate1);
	iDio_SetPortDirection(kPortC,kMaskIo16,kIoOutput);
	//for(i=0;i<50000;i++)
	iDio_SetPort(kPortC,kMaskIo16,kIoOn);
	//iDio_SetPort(kPortC,kMaskIo16,kIoOff);
}

//-----------------------------------------------------------------------------
// Ouverture de l'interface    
//-----------------------------------------------------------------------------
void mIcOc_Open(void)
{

}

//-----------------------------------------------------------------------------
// Fermeture de l'interface   
//-----------------------------------------------------------------------------
void mIcOc_Close(void)
{

}

//------------------------------------------------------------
// Démarre une mesure de distance
//------------------------------------------------------------
void mIcOc_StartDistMesure(void)
{
	iDio_SetPort(kPortC,kMaskIo16,kIoOn);
	// On endort la tâche pour 100ms
	vTaskDelay((UInt32)(2/portTICK_RATE_MS));
	iDio_SetPort(kPortC,kMaskIo16,kIoOff);
}

//------------------------------------------------------------
// Lecture de la distance en cm
//------------------------------------------------------------
float mIcOc_GetDistFront(void)
{
	return iTPM_GetDistFront();
}

//------------------------------------------------------------
// Lecture de la distance en cm
//------------------------------------------------------------
float mIcOc_GetDistBack(void)
{
	return iTPM_GetDistBack();
}

//------------------------------------------------------------
// Lecture de la distance en cm
//------------------------------------------------------------
float mIcOc_GetDistLeft(void)
{
	return iTPM_GetDistLeft();
}

//------------------------------------------------------------
// Lecture de la distance en cm
//------------------------------------------------------------
float mIcOc_GetDistRight(void)
{
	return iTPM_GetDistRight();
}

//-----------------------------------------------------------------------------
// Moteur gauche en avant ou en arrière
// aDuty: pwm duty cycle (0.0 to 1.0 --> 0% to 100%)
// aSens: en avant ou en arrière
//-----------------------------------------------------------------------------
void mIcOc_MoteurGauche(SensEnum aSens,float aDuty)
{
	if(aSens==kEnAvant)
		{
			iTPM_UpdatePWM(kPwmCh1,aDuty);
			iTPM_UpdatePWM(kPwmCh2,0);
		}
	else if(aSens==kEnArriere)
		{
			iTPM_UpdatePWM(kPwmCh2,aDuty);
			iTPM_UpdatePWM(kPwmCh1,0);
		}
}

//-----------------------------------------------------------------------------
// Moteur droit en avant ou en arrière
// aDuty: pwm duty cycle (0.0 to 1.0 --> 0% to 100%)
// aSens: en avant ou en arrière
//-----------------------------------------------------------------------------
void mIcOc_MoteurDroit(SensEnum aSens,float aDuty)
{
	if(aSens==kEnAvant)
		{
			iTPM_UpdatePWM(kPwmCh3,aDuty);
			iTPM_UpdatePWM(kPwmCh4,0);
		}
	else if(aSens==kEnArriere)
		{
			iTPM_UpdatePWM(kPwmCh4,aDuty);
			iTPM_UpdatePWM(kPwmCh3,0);
		}
}

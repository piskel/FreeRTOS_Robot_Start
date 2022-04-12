/*
------------------------------------------------------------
Copyright 2003-20xx Haute école ARC Ingéniérie, Switzerland. 
All rights reserved.
------------------------------------------------------------
Nom du fichier :	iTPM.c
Auteur et Date :	Monnerat Serge 11.01.20xx

Description dans le fichier iTPM.h
------------------------------------------------------------
*/

#include "iTPM.h"
#include <MKL46Z4.h>
#include <core_cm0plus.h>


// Priorité de l'interruption TPM1 et TPM2
#define kTPM2Prio 1
#define kTPM1Prio 2

// Durée d'un step du compteur TPM
#define kTimeFor1Step (float)(622.667E-9)

// Valeur du compteur au flanc montant
static UInt16 sRisingEdgeValFront=0;
// Valeur du compteur au flanc descendant
static UInt16 sFallingEdgeValFront=0;
// Delta T entre les 2 valeurs ci-dessus
static UInt16 sDeltaTFront=0;

// Valeur du compteur au flanc montant
static UInt16 sRisingEdgeValBack=0;
// Valeur du compteur au flanc descendant
static UInt16 sFallingEdgeValBack=0;
// Delta T entre les 2 valeurs ci-dessus
static UInt16 sDeltaTBack=0;

// Valeur du compteur au flanc montant
static UInt16 sRisingEdgeValLeft=0;
// Valeur du compteur au flanc descendant
static UInt16 sFallingEdgeValLeft=0;
// Delta T entre les 2 valeurs ci-dessus
static UInt16 sDeltaTLeft=0;

// Valeur du compteur au flanc montant
static UInt16 sRisingEdgeValRight=0;
// Valeur du compteur au flanc descendant
static UInt16 sFallingEdgeValRight=0;
// Delta T entre les 2 valeurs ci-dessus
static UInt16 sDeltaTRight=0;

// Etat de la machine d'état de l'interruption
typedef enum
{
  kWaitRisingEdge,
  kWaitFallingEdge
}CaptStateEnum;
static CaptStateEnum sCaptStateFront=kWaitRisingEdge;
static CaptStateEnum sCaptStateLeft=kWaitRisingEdge;
static CaptStateEnum sCaptStateBack=kWaitRisingEdge;
static CaptStateEnum sCaptStateRight=kWaitRisingEdge;

//------------------------------------------------------------
// Caractéristique du capteur MaxSonar EZ1
// La largeur de l'impulsion est de 147us par inch soit
// 147us par 25.4 mm
// La plage va de 6 inch à 254 inch soit
// 15.24 cm à 6.4516 m
// Donc l'impulsion varie de 882us à 37.338ms
// On maximise pour 37.338ms --> compteur à 60000
// Ce qui nous donne 1 step du compteur toutes les 622.3 ns --> 1.6MHz
// 24Mhz/1.6Mhz=15 à disposition /16 uniquement
// 24MHz/16=fréquence compteur 1.5MHz --> 1 step toutes les 666.66ns
// --> distance max mesurée par le capteur = 37.338ms = 56007 step et
// --> distance min mesurée par le capteur = 882us = 1323 step
//------------------------------------------------------------


//------------------------------------------------------------
// TPM setup
//------------------------------------------------------------
void iTPM_Config(void)
{
	// System Options Register 2 (SIM_SOPT2)
	// Choix de l'horloge source pour le TPM
	// Source du clock MCGPLLCLK/2, donc 24MHz
	SIM->SOPT2|=SIM_SOPT2_TPMSRC(1)|SIM_SOPT2_PLLFLLSEL_MASK;
	
	// Enable du clock du TPM
	// System Clock Gating Control Register 6 (SIM_SCGC6)
	SIM->SCGC6|=(SIM_SCGC6_TPM0_MASK|SIM_SCGC6_TPM1_MASK|SIM_SCGC6_TPM2_MASK);
	
	// Configuration du crossbar pour les pin IC et OC (PTE22:OC et PTE23:IC)
	// Pin Control Register n (PORTx_PCRn) --> TPM = alternative 3
	PORTE->PCR[22]=(0|PORT_PCR_MUX(3)|PORT_PCR_DSE_MASK);	// IC --> TPM2 channel 0 --> FRONT
	PORTE->PCR[23]=(0|PORT_PCR_MUX(3)|PORT_PCR_DSE_MASK);	// IC --> TPM2 channel 1 --> LEFT
	PORTA->PCR[12]=(0|PORT_PCR_MUX(3)|PORT_PCR_DSE_MASK);	// IC --> TPM1 channel 0 --> BACK
	PORTA->PCR[13]=(0|PORT_PCR_MUX(3)|PORT_PCR_DSE_MASK);	// IC --> TPM1 channel 1 --> RIGHT
	
	PORTA->PCR[4]=(0|PORT_PCR_MUX(3)|PORT_PCR_DSE_MASK);		// PWM --> moteur gauche
	PORTA->PCR[5]=(0|PORT_PCR_MUX(3)|PORT_PCR_DSE_MASK);		// PWM --> moteur gauche
	PORTA->PCR[6]=(0|PORT_PCR_MUX(3)|PORT_PCR_DSE_MASK);		// PWM --> moteur droit
	PORTA->PCR[7]=(0|PORT_PCR_MUX(3)|PORT_PCR_DSE_MASK);		// PWM --> moteur droit
	
	//--------------------------------------------------------------
	// TPM0 --> PWM
	//--------------------------------------------------------------
	// Status and Control (TPMx_SC)
	// Disables DMA transfers
	// Disable TOF interrupts. Use software polling or DMA request
	// TPM counter operates in up counting mode
	// TPM counter increments on every TPM counter clock
	// Prescale Factor Selection: Divide by 1 --> 24MHz/1=24MHz
	TPM0->SC=0|(TPM_SC_CMOD(1)|TPM_SC_PS(0));
	
	// Channel (n) Status and Control (TPMx_CnSC)
	// Edge-aligned PWM setup
	TPM0->CONTROLS[1].CnSC=0|(TPM_CnSC_ELSB_MASK|TPM_CnSC_MSB_MASK);
	TPM0->CONTROLS[2].CnSC=0|(TPM_CnSC_ELSB_MASK|TPM_CnSC_MSB_MASK);
	TPM0->CONTROLS[3].CnSC=0|(TPM_CnSC_ELSB_MASK|TPM_CnSC_MSB_MASK);
	TPM0->CONTROLS[4].CnSC=0|(TPM_CnSC_ELSB_MASK|TPM_CnSC_MSB_MASK);
	
	// Modulo (TPMx_MOD)
	// PWM period setup = 24MHz/(TPM0_MOD)--> TPM0_MOD= 24MHz/20kHz=1200
	TPM0->MOD=1200;
	
	//--------------------------------------------------------------
	// TPM2
	//--------------------------------------------------------------

	// Status and Control (TPMx_SC)
	// Disables DMA transfers
	// Disable TOF interrupts. Use software polling or DMA request
	// TPM counter operates in up counting mode
	// TPM counter increments on every TPM counter clock
	// Prescale Factor Selection: Divide by 16 --> 24MHz/16=1.5MHz --> 1 step toutes les 666.67 ns
	TPM2->SC=0;
	TPM2->SC|=TPM_SC_PS(4);
	TPM2->SC|=TPM_SC_CMOD(1);
	
	// Channel (n) Status and Control (TPMx_CnSC)
	// Rising edge capture et enable de l'interruption
	TPM2->CONTROLS[0].CnSC=0;	// IC
	TPM2->CONTROLS[0].CnSC|=TPM_CnSC_ELSA_MASK;	// IC
	TPM2->CONTROLS[0].CnSC|=TPM_CnSC_CHIE_MASK;	// IC
	TPM2->CONTROLS[1].CnSC=0;	// IC
	TPM2->CONTROLS[1].CnSC|=TPM_CnSC_ELSA_MASK;	// IC
	TPM2->CONTROLS[1].CnSC|=TPM_CnSC_CHIE_MASK;	// IC
	
	// Modulo (TPMx_MOD)
	// Valeur maximale, évite de trop fréquent overflow
	TPM2->MOD=65535;
	
	// Enable de l'interruption TPM2 au niveau du NVIC
	// Le vecteur du TPM2 est le numéro 19
	NVIC_EnableIRQ(TPM2_IRQn);
		
	// Configuration de la priorité de l'interruption TPM2
	// O : plus haute priorité
	// 3 : plus basse priorité
	NVIC_SetPriority(TPM2_IRQn, kTPM2Prio);

	//--------------------------------------------------------------
	// TPM1
	//--------------------------------------------------------------

		
	// Status and Control (TPMx_SC)
	// Disables DMA transfers
	// Disable TOF interrupts. Use software polling or DMA request
	// TPM counter operates in up counting mode
	// TPM counter increments on every TPM counter clock
	// Prescale Factor Selection: Divide by 16 --> 24MHz/16=1.5MHz --> 1 step toutes les 666.67 ns
	TPM1->SC=0;
	TPM1->SC|=TPM_SC_PS(4);
	TPM1->SC|=TPM_SC_CMOD(1);
	
	// Channel (n) Status and Control (TPMx_CnSC)
	// Rising edge capture et enable de l'interruption
	TPM1->CONTROLS[0].CnSC=0;	// IC
	TPM1->CONTROLS[0].CnSC|=TPM_CnSC_ELSA_MASK;	// IC
	TPM1->CONTROLS[0].CnSC|=TPM_CnSC_CHIE_MASK;	// IC
	TPM1->CONTROLS[1].CnSC=0;	// IC
	TPM1->CONTROLS[1].CnSC|=TPM_CnSC_ELSA_MASK;	// IC
	TPM1->CONTROLS[1].CnSC|=TPM_CnSC_CHIE_MASK;	// IC
	
	// Modulo (TPMx_MOD)
	// Valeur maximale, évite de trop fréquent overflow
	TPM1->MOD=65535;

	// Enable de l'interruption TPM1 au niveau du NVIC
	// Le vecteur du TPM1 est le numéro 18
	NVIC_EnableIRQ(TPM1_IRQn);

	// Configuration de la priorité de l'interruption TPM1
	// O : plus haute priorité
	// 3 : plus basse priorité
	NVIC_SetPriority(TPM1_IRQn, kTPM1Prio);
}

//------------------------------------------------------------
// Set PWM duty
//------------------------------------------------------------
void iTPM_UpdatePWM(PwmChannelEnum aChannel, float aDuty)
{
	if(aChannel==kPwmCh1)
		{
			TPM0->CONTROLS[1].CnV=TPM0->MOD*aDuty;
		}
	else if(aChannel==kPwmCh2) 
		{
			TPM0->CONTROLS[2].CnV=TPM0->MOD*aDuty;
		}
	else if(aChannel==kPwmCh3) 
		{
			TPM0->CONTROLS[3].CnV=TPM0->MOD*aDuty;
		}
	else if(aChannel==kPwmCh4) 
		{
			TPM0->CONTROLS[4].CnV=TPM0->MOD*aDuty;
		}
}
//------------------------------------------------------------
// Lecture de la distance en cm
//------------------------------------------------------------
float iTPM_GetDistFront(void)
{    
	return (((sDeltaTFront*kTimeFor1Step)/(147E-6))*2.54);
}

//------------------------------------------------------------
// Lecture de la distance en cm
//------------------------------------------------------------
float iTPM_GetDistBack(void)
{    
	return (((sDeltaTBack*kTimeFor1Step)/(147E-6))*2.54);
}

//------------------------------------------------------------
// Lecture de la distance en cm
//------------------------------------------------------------
float iTPM_GetDistLeft(void)
{    
	return (((sDeltaTLeft*kTimeFor1Step)/(147E-6))*2.54);
}

//------------------------------------------------------------
// Lecture de la distance en cm
//------------------------------------------------------------
float iTPM_GetDistRight(void)
{    
	return (((sDeltaTRight*kTimeFor1Step)/(147E-6))*2.54);
}

//------------------------------------------------------------
// Interruption TPM2
//------------------------------------------------------------
void TPM2_IRQHandler(void)
{
	UInt32 aVal=0;
	
	// Contrôle si l'interruption est due à un  input capture channel 0 --> FRONT
	if((TPM2->CONTROLS[0].CnSC&TPM_CnSC_CHF_MASK)==TPM_CnSC_CHF_MASK)
		{
			// Reset du flag
			TPM2->CONTROLS[0].CnSC|=TPM_CnSC_CHF_MASK;
			
			switch(sCaptStateFront)
				{
					// Capture au flanc montant
					case kWaitRisingEdge:
						// Capture de la valeur du compteur
						sRisingEdgeValFront=TPM2->CONTROLS[0].CnV;
						
						// Configuration pour déclencher au flanc descendant
						TPM2->CONTROLS[0].CnSC&=(~TPM_CnSC_ELSA_MASK);
						TPM2->CONTROLS[0].CnSC|=TPM_CnSC_ELSB_MASK;
						
						// Prochain état
						sCaptStateFront=kWaitFallingEdge;
					break;
					// Capture au flanc descendant
					case kWaitFallingEdge:
						// Capture de la valeur du compteur
						sFallingEdgeValFront=TPM2->CONTROLS[0].CnV;
						
						// Configuration pour déclencher au flanc montant
						TPM2->CONTROLS[0].CnSC&=(~TPM_CnSC_ELSB_MASK);
						TPM2->CONTROLS[0].CnSC|=TPM_CnSC_ELSA_MASK;
						
						// Calcul du Delta T
						if(sFallingEdgeValFront<sRisingEdgeValFront)
							{
								sDeltaTFront=(65535-sRisingEdgeValFront)+sFallingEdgeValFront;
							}
						else
							{
								sDeltaTFront=sFallingEdgeValFront-sRisingEdgeValFront;
							}
						
						// Prochain état
						sCaptStateFront=kWaitRisingEdge;
					break;
				}
		}
		
	// Contrôle si l'interruption est due à un  input capture channel 1 --> LEFT
	if((TPM2->CONTROLS[1].CnSC&TPM_CnSC_CHF_MASK)==TPM_CnSC_CHF_MASK)
		{
			// Reset du flag
			TPM2->CONTROLS[1].CnSC|=TPM_CnSC_CHF_MASK;
			
			switch(sCaptStateLeft)
				{
					// Capture au flanc montant
					case kWaitRisingEdge:
						// Capture de la valeur du compteur
						sRisingEdgeValLeft=TPM2->CONTROLS[1].CnV;
						
						// Configuration pour déclencher au flanc descendant
						TPM2->CONTROLS[1].CnSC&=(~TPM_CnSC_ELSA_MASK);
						TPM2->CONTROLS[1].CnSC|=TPM_CnSC_ELSB_MASK;
						
						// Prochain état
						sCaptStateLeft=kWaitFallingEdge;
					break;
					// Capture au flanc descendant
					case kWaitFallingEdge:
						// Capture de la valeur du compteur
						sFallingEdgeValLeft=TPM2->CONTROLS[1].CnV;
						
						// Configuration pour déclencher au flanc montant
						TPM2->CONTROLS[1].CnSC&=(~TPM_CnSC_ELSB_MASK);
						TPM2->CONTROLS[1].CnSC|=TPM_CnSC_ELSA_MASK;
						
						// Calcul du Delta T
						if(sFallingEdgeValLeft<sRisingEdgeValLeft)
							{
								sDeltaTLeft=(65535-sRisingEdgeValLeft)+sFallingEdgeValLeft;
							}
						else
							{
								sDeltaTLeft=sFallingEdgeValLeft-sRisingEdgeValLeft;
							}
						
						// Prochain état
						sCaptStateLeft=kWaitRisingEdge;
					break;
				}
		}

}

//------------------------------------------------------------
// Interruption TPM1
//------------------------------------------------------------
void TPM1_IRQHandler(void)
{
	UInt32 aVal=0;
	static UInt16 sOverflowCounter=0;
	
	// Contrôle si l'interruption est due à un  input capture channel 0 --> BACK
	if((TPM1->CONTROLS[0].CnSC&TPM_CnSC_CHF_MASK)==TPM_CnSC_CHF_MASK)
		{
			// Reset du flag
			TPM1->CONTROLS[0].CnSC|=TPM_CnSC_CHF_MASK;
						
			switch(sCaptStateBack)
				{
					// Capture au flanc montant
					case kWaitRisingEdge:
						// Capture de la valeur du compteur
						sRisingEdgeValBack=TPM1->CONTROLS[0].CnV;
						
						// Configuration pour déclencher au flanc descendant
						TPM1->CONTROLS[0].CnSC&=(~TPM_CnSC_ELSA_MASK);
						TPM1->CONTROLS[0].CnSC|=TPM_CnSC_ELSB_MASK;
						
						// Prochain état
						sCaptStateBack=kWaitFallingEdge;
					break;
					// Capture au flanc descendant
					case kWaitFallingEdge:
						// Capture de la valeur du compteur
						sFallingEdgeValBack=TPM1->CONTROLS[0].CnV;
						
						// Configuration pour déclencher au flanc montant
						TPM1->CONTROLS[0].CnSC&=(~TPM_CnSC_ELSB_MASK);
						TPM1->CONTROLS[0].CnSC|=TPM_CnSC_ELSA_MASK;
						
						// Calcul du Delta T
						if(sFallingEdgeValBack<sRisingEdgeValBack)
							{
								sDeltaTBack=(65535-sRisingEdgeValBack)+sFallingEdgeValBack;
							}
						else
							{
								sDeltaTBack=sFallingEdgeValBack-sRisingEdgeValBack;
							}
						
						// Prochain état
						sCaptStateBack=kWaitRisingEdge;
					break;
				}
		}
		
	// Contrôle si l'interruption est due à un  input capture channel 1 --> RIGHT
	if((TPM1->CONTROLS[1].CnSC&TPM_CnSC_CHF_MASK)==TPM_CnSC_CHF_MASK)
		{
			// Reset du flag
			TPM1->CONTROLS[1].CnSC|=TPM_CnSC_CHF_MASK;
			
			switch(sCaptStateRight)
				{
					// Capture au flanc montant
					case kWaitRisingEdge:
						// Capture de la valeur du compteur
						sRisingEdgeValRight=TPM1->CONTROLS[1].CnV;
						
						// Configuration pour déclencher au flanc descendant
						TPM1->CONTROLS[1].CnSC&=(~TPM_CnSC_ELSA_MASK);
						TPM1->CONTROLS[1].CnSC|=TPM_CnSC_ELSB_MASK;
						
						// Prochain état
						sCaptStateRight=kWaitFallingEdge;
					break;
					// Capture au flanc descendant
					case kWaitFallingEdge:
						// Capture de la valeur du compteur
						sFallingEdgeValRight=TPM1->CONTROLS[1].CnV;
						
						// Configuration pour déclencher au flanc montant
						TPM1->CONTROLS[1].CnSC&=(~TPM_CnSC_ELSB_MASK);
						TPM1->CONTROLS[1].CnSC|=TPM_CnSC_ELSA_MASK;
						
						// Calcul du Delta T
						if(sFallingEdgeValRight<sRisingEdgeValRight)
							{
								sDeltaTRight=(65535-sRisingEdgeValRight)+sFallingEdgeValRight;
							}
						else
							{
								sDeltaTRight=sFallingEdgeValRight-sRisingEdgeValRight;
							}
						
						// Prochain état
						sCaptStateRight=kWaitRisingEdge;
					break;
				}
		}
}







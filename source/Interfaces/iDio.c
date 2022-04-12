/*
------------------------------------------------------------
Copyright 2003-20xx Haute école ARC Ingéniérie, Switzerland. 
All rights reserved.
------------------------------------------------------------
Nom du fichier :	iDio.c
Auteur et Date :	Monnerat Serge 11.01.20xx

Description dans le fichier iDio.h
------------------------------------------------------------
*/

#include "iDio.h"
#include <MKL46Z4.h>
//-------------------------------------------------------------------------
// Déclaration des constantes
//-------------------------------------------------------------------------
// Registres de direction
#define kGPIOA_PDDR     0x400FF014
#define kGPIOB_PDDR     0x400FF054
#define kGPIOC_PDDR     0x400FF094
#define kGPIOD_PDDR     0x400FF0D4
#define kGPIOE_PDDR     0x400FF114

// Registre de données --> sortie
#define kGPIOA_PDOR     0x400FF000
#define kGPIOB_PDOR     0x400FF040
#define kGPIOC_PDOR     0x400FF080
#define kGPIOD_PDOR     0x400FF0C0
#define kGPIOE_PDOR     0x400FF100

// Registre de données --> entrée
#define kGPIOA_PDIR     0x400FF010
#define kGPIOB_PDIR     0x400FF050
#define kGPIOC_PDIR     0x400FF090
#define kGPIOD_PDIR     0x400FF0D0
#define kGPIOE_PDIR     0x400FF110

// Registre permettant de configurer la fonctionnalité des pins
#define kPORTA_PCR0			0x40049000
#define kPORTB_PCR0			0x4004A000
#define kPORTC_PCR0			0x4004B000
#define kPORTD_PCR0			0x4004C000
#define kPORTE_PCR0			0x4004D000

//-----------------------------------------------------------------------------
// Enable du clock des GPIO
//-----------------------------------------------------------------------------
void iDio_EnablePortClk(void)
{
		// Pointeur permettant d'accéder aux registres d'enable des clocks des port IO
	unsigned long *aSIMSCGC5Addr=(unsigned long*)0x40048038;	
	
	// ------------------------------------------------------------
	// Enable du clock des ports IO
	// ------------------------------------------------------------	
	*aSIMSCGC5Addr=(0|(1<<9)|(1<<10)|(1<<11)|(1<<12)|(1<<13));
}

//-----------------------------------------------------------------------------
// Configuration de la fonctionnalité de chaque PIN
//-----------------------------------------------------------------------------
void iDio_PinConfig(PortIOEnum aPort,PinNbEnum aPinNb, PinFunctionEnum aPinFunc)
{
	unsigned long *aPortPtr;
	
	// Configuration du pointeur selon le port passé en paramètre
	switch(aPort)
		{
			// DDRA
			case kPortA:
				aPortPtr=(unsigned long *)(kPORTA_PCR0+(aPinNb*4));
				
			break;
			
			// DDRB
			case kPortB:
				aPortPtr=(unsigned long *)(kPORTB_PCR0+(aPinNb*4));
			break;
			
			// DDRC
			case kPortC:
				aPortPtr=(unsigned long *)(kPORTC_PCR0+(aPinNb*4));
			break;
			
			// DDRD
			case kPortD:
				aPortPtr=(unsigned long *)(kPORTD_PCR0+(aPinNb*4));
			break;
			
			// DDRE
			 case kPortE:
				 aPortPtr=(unsigned long *)(kPORTE_PCR0+(aPinNb*4));
			 break;
		}     
	
	// Reset du champ MUX
	*aPortPtr&=0xFFFFF8FF;
	// Configuration de la fonction
	*aPortPtr|=aPinFunc;
}

//-----------------------------------------------------------------------------
// Configuration des Ports en entrée ou en sortie
// aPort: le port IO que l'on veut configurer
// aMask: choix des pins du port sur lesquelles on veut agir (si bit à 1)
// aDir:  choix de la direction des pins sélectionnées avec aMask
//-----------------------------------------------------------------------------
void iDio_SetPortDirection(PortIOEnum aPort,IoMaskEnum aMask,IoDirectionEnum aDir)
{
  unsigned long *aPortPtr;
  
  
  // Configuration du pointeur selon le port passé en paramètre
  switch(aPort)
    {
      // DDRA
      case kPortA:
        aPortPtr=(unsigned long *)(kGPIOA_PDDR);
      break;
      
      // DDRB
      case kPortB:
        aPortPtr=(unsigned long *)(kGPIOB_PDDR);
      break;
      
      // DDRC
      case kPortC:
        aPortPtr=(unsigned long *)(kGPIOC_PDDR);
      break;
      
      // DDRD
      case kPortD:
        aPortPtr=(unsigned long *)(kGPIOD_PDDR);
      break;
      
      // DDRE
       case kPortE:
         aPortPtr=(unsigned long *)(kGPIOE_PDDR);
       break;
    } 
    
  // Configuration des bits du port désiré en entrée ou sortie
  if(kIoOutput==aDir)
    {
      *aPortPtr|=aMask;
    }
  else if(kIoInput==aDir)
    {
      *aPortPtr &=~aMask;
    } 
}


//-----------------------------------------------------------------------------
// Affectation de l'état des pins du port configurées en sortie
// aMask:   choix des pins du port sur lesquelles on veut agir (si bit à 1)
// aState:  choix de l'état des bits du port désirés
//-----------------------------------------------------------------------------
void iDio_SetPort(PortIOEnum aPort,IoMaskEnum aMask,IoStateEnum aState)
{
  unsigned long *aPortPtr;
  
  // Configuration du pointeur selon le port passé en paramètre
  switch(aPort)
    {
      // DDRA
      case kPortA:
        aPortPtr=(unsigned long *)(kGPIOA_PDOR);
      break;
      
      // DDRB
      case kPortB:
        aPortPtr=(unsigned long *)(kGPIOB_PDOR);
      break;
      
      // DDRC
      case kPortC:
        aPortPtr=(unsigned long *)(kGPIOC_PDOR);
      break;
      
      // DDRD
      case kPortD:
        aPortPtr=(unsigned long *)(kGPIOD_PDOR);
      break;
      
      // DDRE
       case kPortE:
         aPortPtr=(unsigned long *)(kGPIOE_PDOR);
       break;
    } 
    
  // Configuration de l'état des bits du port configurés en sortie
  if(kIoOn==aState)
    {
      *aPortPtr|=aMask;
    }
  else if(kIoOff==aState)
    {
      *aPortPtr&=~aMask;
    }
}

//-----------------------------------------------------------------------------
// Lecture de l'état des pins du port configurées en entrée
// aMask:   de quelle pin ou veut l'état 
// retour:  état de la pin --> true ou false
//-----------------------------------------------------------------------------
bool iDio_GetPort(PortIOEnum aPort,IoMaskEnum aMask)
{
  bool  aRet=false;
  unsigned long *aPortPtr;
  
  // Configuration du pointeur selon le port passé en paramètre
  switch(aPort)
    {
      // DDRA
      case kPortA:
        aPortPtr=(unsigned long *)(kGPIOA_PDIR);
      break;
      
      // DDRB
      case kPortB:
        aPortPtr=(unsigned long *)(kGPIOB_PDIR);
      break;
      
      // DDRC
      case kPortC:
        aPortPtr=(unsigned long *)(kGPIOC_PDIR);
      break;
      
      // DDRD
      case kPortD:
        aPortPtr=(unsigned long *)(kGPIOD_PDIR);
      break;
      
      // DDRE
       case kPortE:
         aPortPtr=(unsigned long *)(kGPIOE_PDIR);
       break;
    }     
 
  // Lecture de l'état des bit du port configurés en entrée
  aRet=(bool)((*aPortPtr&aMask)==aMask);
  
  return aRet;
}

//-----------------------------------------------------------------------------
// Le lien entre la donnée ou la commande à transmettre et les bits du port E et B
// aVal:   la commande ou la donnée
//-----------------------------------------------------------------------------
void iDio_PortLcd(unsigned char aVal)
{
	// Bit0,	Bit1,		Bit2,		Bit3,		Bit4,		Bit5,		Bit6,		Bit7,		
	// PTE19,	PTE18, 	PTE6 ,	PTE3,		PTE2,	  PTE20,	PTE21,	PTE30,	
	
	// Reset des bits du port E utilisés par le LCD
	GPIOE->PDOR&=(~(0|((1<<19)|(1<<18)|(1<<6)|(1<<3)|(1<<2)|(1<<20)|(1<<21)|(1<<30))));
	
	// Set des bits du port E utilisés par le LCD
	// Example: Data1 --> bit 21 du port E --> déplacement de 21-1 sur la gauche
	// Example: Data6 --> bit 18 du port E --> déplacement de 18-6 sur la gauche
	GPIOE->PDOR|=(((aVal&0x01)<<19)|((aVal&0x02)<<17)|((aVal&0x04)<<4)|((aVal&0x08)>>0)|((aVal&0x10)>>2)|((aVal&0x20)<<15)|((aVal&0x40)<<15)|((aVal&0x80)<<23));
}

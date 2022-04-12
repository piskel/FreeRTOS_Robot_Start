/*
------------------------------------------------------------
Copyright 2003-20xx Haute école ARC Ingéniérie, Switzerland. 
All rights reserved.
------------------------------------------------------------
Nom du fichier :	mCpu.c
Auteur et Date :	Monnerat Serge 11.01.20xx

Description dans le fichier mCpu.h
------------------------------------------------------------
*/


#include "mCpu.h"
#include "iCpu.h"

//-----------------------------------------------------------------------
// Congiguration générale de la CPU
//-----------------------------------------------------------------------
void mCpu_Setup(void)
{
	// Configuration du clock de la CPU et du bus
	iCpu_CLKInit();
}


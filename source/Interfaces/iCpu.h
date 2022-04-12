/*
------------------------------------------------------------
Copyright 2003-2014 Haute école ARC Ingéniérie, Switzerland.
All rights reserved.
------------------------------------------------------------
Nom du fichier : 	iCpu.h
Auteur et Date :	Monnerat Serge 7 mars 2014

But : Interface permettant l'utilisation du CPU

-----------------------------------------------------------------------------
Historique:
-----------------------------------------------------------------------------

$History: $


-----------------------------------------------------------------------------
*/


#ifndef __ICPU__
#define __ICPU__

#include "def.h"

//------------------------------------------------------------
// Core and peripheral clock init
// External clock = 8MHz
// Core clock = 48MHz
// Peripheral bus = 24MHz
// Flexbus = 24MHz
// Flash = 24MHz
//------------------------------------------------------------
void iCpu_CLKInit(void);

#endif

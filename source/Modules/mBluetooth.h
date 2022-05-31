 /*
 * mBluetooth.h
 *
 *  Created on: Apr 24, 2014
 *      Author: serge.monnerat
 */

#ifndef __MBLUETOOTH__
#define __MBLUETOOTH__

#include "def.h"

//-------------------------------------------------------------------------
// Configuration du module SCI   
//-------------------------------------------------------------------------
void mBluetooth_Setup(void);

//-------------------------------------------------------------------------
// Ouverture des interfaces    
//-------------------------------------------------------------------------
void mBluetooth_Open(void);

//-------------------------------------------------------------------------
// Attends une connexion Bluetooth
//-------------------------------------------------------------------------
void mBluetooth_WaitConnect(void);

//-------------------------------------------------------------------------
// Attends le message OK du Bluetooth
//-------------------------------------------------------------------------
void mBluetooth_WaitOK(void);

//-------------------------------------------------------------------------
// Attends le message PAIRED du Bluetooth
//-------------------------------------------------------------------------
void mBluetooth_WaitPaired(void);

//-------------------------------------------------------------------------
// Envoi d'un caractère par RS232
// aChar	: la caractère à transmettre
//-------------------------------------------------------------------------
void mBluetooth_WriteChar(Int8 aChar);

//-------------------------------------------------------------------------
// Envoi d'une chaîne de caractères par RS232
// La tramsmission s'arrête au caractère NULL
// *aDataPtr	: pointeur sur la chaîne de caractères à transmettre, la chaîne
//              doit-être terminée par le caractère NULL("ma chaîne")
//-------------------------------------------------------------------------
void mBluetooth_WriteString(Int8 *aDataPtr);

//-----------------------------------------------------------------------------
// Lecture d'un byte du buffer de réception. Le buffer est rempli par 
// l'interruption Receive data reg full
// *aBytePtr	  : pointeur sur la variable recevant la donnée
// Retour 	    : true --> le buffer est vide, false le buffer est pas vide
//-----------------------------------------------------------------------------
bool mBluetooth_ReadDataFromBuffer(UInt8 *aBytePtr);


#endif

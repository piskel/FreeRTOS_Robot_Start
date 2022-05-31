/*
 * mBluetooth.c
 *
 *  Created on: Apr 24, 2014
 *      Author: serge.monnerat
 */

#include "iUart.h"
#include "iPit.h"
#include "iDio.h"
#include "mBluetooth.h"

//-------------------------------------------------------------------------
// Configuration du module SCI   
//-------------------------------------------------------------------------
void mBluetooth_Setup(void)
    {
    //------------------------------------------------------------
    // Configuration du port RS232
    //------------------------------------------------------------
    iUart_Config(kUart1);

    // Init du buffer de r�ception avec 0
    iUart_InitRxBuffer(kUart1, 0);

//     Pin 1 du port A utilis�e pour le passage AT mode <--> paired mode
    iDio_PinConfig(kPortC, kPin13, kAlternate1);
    iDio_SetPort(kPortC, kMaskIo13, kIoOff);
    iDio_SetPortDirection(kPortC, kMaskIo13, kIoOutput);
    }

//-------------------------------------------------------------------------
// Ouverture des interfaces    
//-------------------------------------------------------------------------
void mBluetooth_Open(void)
    {
    Int16 aDly;

    iUart_EnDisTx(kUart1, kEn);
    iUart_EnDisRx(kUart1, kEn);

    iDio_SetPort(kPortC, kMaskIo13, kIoOn);

    // Est-ce que le module r�pond
    mBluetooth_WriteString("AT\r\n");

    // Mode SLAVE --> Bluetooth Terminal HC-05 sur Android
    mBluetooth_WriteString("AT+ORGL\r\n");
    mBluetooth_WaitOK();

    // Lecture de la version du firmware
    mBluetooth_WriteString("AT+VERSION?\r\n");
    mBluetooth_WaitOK();

    // Lecture de l'adresse MAC du module
    mBluetooth_WriteString("AT+ADDR?\r\n");
    mBluetooth_WaitOK();

    // D� appaire tous les modules pr�c�demment appair�s
    mBluetooth_WriteString("AT+RMAAD\r\n");
    mBluetooth_WaitOK();

    // Setup du nom du module
    mBluetooth_WriteString("AT+NAME=Robot Luc\r\n");
    mBluetooth_WaitOK();

    // Setup du mode slave --> le PC se connecte
    mBluetooth_WriteString("AT+ROLE=0\r\n");
    mBluetooth_WaitOK();

    // Setup du password --> 0000
    mBluetooth_WriteString("AT+PSWD=0000\r\n");
    mBluetooth_WaitOK();

    mBluetooth_WriteString("AT+RESET\r\n");

    // Passage en mode paired, le PC peut "voir" le module
    iDio_SetPort(kPortC, kMaskIo13, kIoOff);
    iUart_InitRxBuffer(kUart1, 0);
    }

//-------------------------------------------------------------------------
// Attends une connexion Bluetooth
// 1. La 1�re fois choisir Ajouter un p�riph�rique Bluetooth sur votre PC
// 2. Connecter un terminal sur le port com alouer au Bluetoothg
// 3. Entrer le code --> 0000
// Par la suite seul les point 2 et 3 sont n�cessaire
//-------------------------------------------------------------------------
void mBluetooth_WaitConnect(void)
    {
    UInt16 aByteCounter;
    UInt8 aByte;
    UInt8 aRs232Buff[200];
    UInt16 i;

    // Reset du buffer
    for (i = 0; i < 200; i++)
	aRs232Buff[i] = 0;

    // Contr�le si un caract�re est re�u
    aByteCounter = 0;
    while ((aRs232Buff[aByteCounter - 1] != 'T')
	    || (aRs232Buff[aByteCounter - 2] != 'C'))
	{
	// Enregistrement du byte dans le buffer
	if (mBluetooth_ReadDataFromBuffer(&aByte) == false)
	    {
	    aRs232Buff[aByteCounter] = aByte;
	    // Compteur de bytes re�us
	    aByteCounter++;
	    }
	}
    }

//-------------------------------------------------------------------------
// Attends le message OK du Bluetooth
//-------------------------------------------------------------------------
void mBluetooth_WaitOK(void)
    {
    UInt16 aByteCounter;
    UInt8 aByte;
    UInt8 aRs232Buff[200];
    UInt16 i;

    // Reset du buffer
    for (i = 0; i < 200; i++)
	aRs232Buff[i] = 0;

    // Contr�le si un caract�re est re�u
    aByteCounter = 0;
    while ((aRs232Buff[aByteCounter - 1] != 'K')
	    || (aRs232Buff[aByteCounter - 2] != 'O'))
	{
	// Enregistrement du byte dans le buffer
	if (mBluetooth_ReadDataFromBuffer(&aByte) == false)
	    {
	    aRs232Buff[aByteCounter] = aByte;
	    // Compteur de bytes re�us
	    aByteCounter++;
	    }
	}
    }

//-------------------------------------------------------------------------
// Envoi d'un caract�re par RS232
// aChar	: la caract�re � transmettre
//-------------------------------------------------------------------------
void mBluetooth_WriteChar(Int8 aChar)
    {
    // Attente fin d'envoi
    while (!iUart_GetStatus(kUart1, kTransmitComplete))
	;

    // Ecriture du registre de donn�es
    iUart_SetData(kUart1, aChar);
    }

//-------------------------------------------------------------------------
// Envoi d'une cha�ne de caract�res par RS232
// La tramsmission s'arr�te au caract�re NULL
// *aDataPtr	: pointeur sur la cha�ne de caract�res � transmettre, la cha�ne
//              doit-�tre termin�e par le caract�re NULL("ma cha�ne")
//-------------------------------------------------------------------------
void mBluetooth_WriteString(Int8 *aDataPtr)
    {
    // Tant que toutes les donn�es ne sont pas �mises
    while (*aDataPtr != 0)
	{
	// Attente fin d'envoi
	while (!iUart_GetStatus(kUart1, kTransmitComplete))
	    ;

	// Ecriture du registre de donn�es
	iUart_SetData(kUart1, *aDataPtr);

	// prochain byte du buffer
	aDataPtr++;
	}
    }

//-----------------------------------------------------------------------------
// Lecture d'un byte du buffer de r�ception. Le buffer est rempli par 
// l'interruption Receive data reg full
// *aBytePtr	  : pointeur sur la variable recevant la donn�e
// Retour 	    : true --> le buffer est vide, false le buffer est pas vide
//-----------------------------------------------------------------------------
bool mBluetooth_ReadDataFromBuffer(UInt8 *aBytePtr)
    {
    bool aRet = false;

    // Contr�le si le buffer est vide
    aRet = iUart_IsBufferEmpty(kUart1);

    // Si le buffer n'est pas vide --> lecture d'un byte
    if (false == aRet)
	{
	// Lecture d'un byte du buffer de r�ception
	*aBytePtr = iUart_GetCharFromBuffer(kUart1);
	}

    return aRet;
    }



#include "mBT.h"

#include "MKL46Z4.h"
#include "stdio.h"

#include "iDio.h"
#include "iUart.h"

#include "string.h"



void mBT_Setup(void)
    {

    // Digital output for config/com mode switching
//    mDelay_Setup();
//    iDio_EnablePortClk();

    iUart_Config(kUart1);

    DisableInterrupts;
    iUart_InitRxBuffer(kUart1,0);
    EnableInterrupts;

    iDio_PinConfig(kPortC, kPin13, kAlternate1);
    iDio_SetPort(kPortC, kMaskIo13, kIoOff);
    iDio_SetPortDirection(kPortC, kMaskIo13, kIoOutput);


    }

void mBT_Start(void)
    {

    iUart_EnDisTx(kUart1,kEn); // Enable TX
    iUart_EnDisRx(kUart1,kEn); // Enable RX

    iDio_SetPort(kPortC,kMaskIo13,kIoOn); // AT mode

//    printf("BT - Reset to default: ");
//    do{
//    mBT_WriteString("AT+ORGL\r\n"); // Reset to default
//    }while(!mBT_WaitOK());
//    printf("OK\n");
//
//    printf("BT - Delete previous devices: ");
//    do{
//    mBT_WriteString("AT+RMAAD\r\n"); // Deletes previously paired devices
//    }while(!mBT_WaitOK());
//    printf("OK\n");

    printf("BT - Setting device name: ");
    do{
    mBT_WriteString("AT+NAME=RBT_00\r\n"); // Sets the device name
    }while(!mBT_WaitOK());
    printf("OK\n");

    printf("BT - Resetting: ");
    mBT_WriteString("AT+RESET\r\n");
    printf("OK\n");

    iDio_SetPort(kPortC, kMaskIo13, kIoOff); // Exiting AT mode

    DisableInterrupts;
    iUart_InitRxBuffer(kUart1,0); // Clearing buffer
    EnableInterrupts;

    printf("BT - Configuration done!\n");
    }

void mBT_Stop(void)
    {

    }


void mBT_WriteChar(char data)
    {
    while(!iUart_GetStatus(kUart1, kTransmitComplete));

    iUart_SetData(kUart1, data);
    }

void mBT_WriteString(char* data)
    {
    for(int i = 0; i < strlen(data); i++)
	{
	    mBT_WriteChar(data[i]);
	}
    }

void mBT_WriteData(char* data, int size)
    {
    for(int i = 0; i < size; i++)
	{
	mBT_WriteChar(data[i]);
	}
    }


bool mBT_ReadData(char *data)
    {
	if (iUart_IsBufferEmpty(kUart1))
	    return false;

	*data = iUart_GetCharFromBuffer(kUart1);

	return true;
    }



bool mBT_WaitOK(void)
    {
    char buffer[2] = {0};

    do
	{
	if(strcmp(buffer, "OK") == 0)
	    {
	    return true;
	    }
	buffer[0] = buffer[1];

	}
    while(mBT_ReadData(&buffer[1]));

    return false;

    }



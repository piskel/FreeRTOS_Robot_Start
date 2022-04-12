

#include "MKL46Z4.h"

#include "iDio.h"
#include "iUart.h"




void mBT_Setup(void)
    {

    // Digital output for config/com mode switching
    mDelay_Setup();
    iDio_EnablePortClk();
    iDio_PinConfig(kPortC, kPin13, kAlternate1);
    iDio_SetPortDirection(kPortC, kMaskIo13, kIoOutput);
    iDio_SetPort(kPortC, kMaskIo13, kIoOn);



    iUart_Config(kUart1);




    }

void mBT_Start(void)
    {

    }

void mBT_Stop(void)
    {

    }


void mBT_WriteChar(char data)
    {

    }

void mBT_WriteString(char* data)
    {

    }

void mBT_WriteData(char* data, int size)
    {

    }


char* mBT_ReadData(void)
    {

    }

void mBT_ClearBuffer(void)
    {

    }



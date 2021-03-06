/*
 * tInputTask.c
 *
 *  Created on: 22 févr. 2022
 *      Author: luc.froideva
 */


#include "tInputTask.h"

#include "FreeRTOS.h"
#include "MKL46Z4.h"
#include "event_groups.h"
#include "stdio.h"
#include "stdlib.h"

#include "def.h"

#include "mSwitch.h"
#include "mIcOc.h"
#include "mLine.h"
#include "mIMU.h"
//#include "mBluetooth.h"
#include "mBT.h"

#include "external.h"


void tInputTask(void *pvParameters)
    {
    (void) pvParameters;

    InputStruct gInputStruct;


    sInputTaskQueue = xQueueCreate(1, sizeof(InputStruct)); // Single element queue for latest values acquired
    vQueueAddToRegistry(sInputTaskQueue, "InputStruct");

    sInputTaskEvents = xEventGroupCreate(); // Flag group to notify when new values have been added

    mIcOc_StartDistMesure(); // Start distance measurement

    char charVal = 0;
    char buffer[3] = {0};
    int bufferIndex = 0;
    bool isReceiving = false;


    while (TRUE)
	{

//	printf("Input task\n");



	///////////////////////////////////////////////////////
	// Distance sensor ////////////////////////////////////
	///////////////////////////////////////////////////////


	gInputStruct.frontDistance = mIcOc_GetDistFront();
	gInputStruct.backDistance  = mIcOc_GetDistBack();
	gInputStruct.leftDistance  = mIcOc_GetDistLeft();
	gInputStruct.rightDistance = mIcOc_GetDistRight();


	///////////////////////////////////////////////////////
	// Bluetooth //////////////////////////////////////////
	///////////////////////////////////////////////////////



	gInputStruct.command = 0;
	gInputStruct.commandArg = 0;

//	DisableInterrupts;
	while(mBT_ReadData(&charVal))
	    {
//	    printf("BT - BF IND : %d\n", bufferIndex);
//	    printf("BT - Data : %X\n", charVal);

	    if(isReceiving)
		{
		buffer[bufferIndex] = charVal;
		bufferIndex++;
		}

	    if (charVal == 0x01 && !isReceiving) // Start of transmission
		{
		bufferIndex = 0;
		isReceiving = true;
		printf("BT - Receiving command...\n");
		}
	    else if(charVal == 0xff && bufferIndex == 3) // End of transmission
		{
		isReceiving = false;
		bufferIndex = 0;

		gInputStruct.command = buffer[0];
		gInputStruct.commandArg = buffer[1];
		printf("BT - Received command : %X %X\n", buffer[0], buffer[1]);

		}
	    else if(bufferIndex > 2)
		{
		isReceiving = false;
		bufferIndex = 0;
		}

	}
//	EnableInterrupts;







	///////////////////////////////////////////////////////
	// Buttons and switches ///////////////////////////////
	///////////////////////////////////////////////////////


	gInputStruct.switchValues =  (mSwitch_ReadSwitch(kSw0) 	 	 & 1) ||
				    ((mSwitch_ReadSwitch(kSw1) << 1) 	 & 1) ||
				    ((mSwitch_ReadSwitch(kSw2) << 2) 	 & 1) ||
				    ((mSwitch_ReadSwitch(kSw3) << 3) 	 & 1) ||
				    ((mSwitch_ReadPushBut(kPushButSW0) << 4) & 1) ||
				    ((mSwitch_ReadPushBut(kPushButSW1) << 5) & 1) ||
				    ((mSwitch_ReadPushBut(kPushButSW2) << 6) & 1) ||
				    ((mSwitch_ReadPushBut(kPushButSW3) << 7) & 1);


	///////////////////////////////////////////////////////
	// Line sensor ////////////////////////////////////////
	///////////////////////////////////////////////////////

	gInputStruct.getPixelValuesError = mLine_GetRaw(&gInputStruct.pixelValues);
	gInputStruct.pixelValues = ~gInputStruct.pixelValues;


	///////////////////////////////////////////////////////
	// IMU ////////////////////////////////////////////////
	///////////////////////////////////////////////////////

	gInputStruct.IMUAccX = mIMU_GetAccX();
	gInputStruct.IMUAccY = mIMU_GetAccY();
	gInputStruct.IMUAccZ = mIMU_GetAccZ();

	gInputStruct.IMUGyroX = mIMU_GetGyroX();
	gInputStruct.IMUGyroY = mIMU_GetGyroY();
	gInputStruct.IMUGyroZ = mIMU_GetGyroZ();


	mIcOc_StartDistMesure(); // Start distance measurement


	// Sending new data to the queue
	xQueueOverwrite(sInputTaskQueue, &gInputStruct);
	// Setting the "new data" flag
	xEventGroupSetBits(sInputTaskEvents, kInputTaskNewDataFlag);

	vTaskDelay(kInputTaskDelay);
	}

    }

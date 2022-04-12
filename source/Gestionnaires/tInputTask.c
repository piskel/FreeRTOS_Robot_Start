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

#include "def.h"

#include "mSwitch.h"
#include "mIcOc.h"
#include "mLine.h"
#include "mIMU.h"

#include "external.h"


void tInputTask(void *pvParameters)
    {
    (void) pvParameters;

    InputStruct gInputStruct;


    sInputTaskQueue = xQueueCreate(1, sizeof(InputStruct)); // Single element queue for latest values acquired
    sInputTaskEvents = xEventGroupCreate(); // Flag group to notify when new values have been added



    while (TRUE)
	{
	mIcOc_StartDistMesure(); // Start distance measurement

	gInputStruct.switchValues =  (mSwitch_ReadSwitch(kSw0) 	 	 & 1) ||
				    ((mSwitch_ReadSwitch(kSw1) << 1) 	 & 1) ||
				    ((mSwitch_ReadSwitch(kSw2) << 2) 	 & 1) ||
				    ((mSwitch_ReadSwitch(kSw3) << 3) 	 & 1) ||
				    ((mSwitch_ReadPushBut(kPushButSW0) << 4) & 1) ||
				    ((mSwitch_ReadPushBut(kPushButSW1) << 5) & 1) ||
				    ((mSwitch_ReadPushBut(kPushButSW2) << 6) & 1) ||
				    ((mSwitch_ReadPushBut(kPushButSW3) << 7) & 1);

	gInputStruct.getPixelValuesError = mLine_GetRaw(&gInputStruct.pixelValues);
	gInputStruct.pixelValues = ~gInputStruct.pixelValues;


	///////////////////////////////////////////////////////
	// Distance sensor ////////////////////////////////////
	///////////////////////////////////////////////////////

	gInputStruct.frontDistance = mIcOc_GetDistFront();
	gInputStruct.backDistance  = mIcOc_GetDistBack();
	gInputStruct.leftDistance  = mIcOc_GetDistLeft();
	gInputStruct.rightDistance = mIcOc_GetDistRight();

	///////////////////////////////////////////////////////
	// IMU ////////////////////////////////////////////////
	///////////////////////////////////////////////////////

	gInputStruct.IMUAccX = mIMU_GetAccX();
	gInputStruct.IMUAccY = mIMU_GetAccY();
	gInputStruct.IMUAccZ = mIMU_GetAccZ();

	gInputStruct.IMUGyroX = mIMU_GetGyroX();
	gInputStruct.IMUGyroY = mIMU_GetGyroY();
	gInputStruct.IMUGyroZ = mIMU_GetGyroZ();


	printf("New Data\n");


//	xEventGroupClearBits(sInputTaskEvents, kInputTaskNewDataFlag);

	// Sending new data to the queue
	xQueueOverwrite(sInputTaskQueue, &gInputStruct);
	// Setting the "new data" flag
	xEventGroupSetBits(sInputTaskEvents, kInputTaskNewDataFlag);

	vTaskDelay(kInputTaskDelay);
	}

    }

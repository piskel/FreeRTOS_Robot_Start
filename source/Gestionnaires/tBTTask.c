/*
 * tHMITask.c
 *
 *  Created on: 22 févr. 2022
 *      Author: luc.froideva
 */

#include "tBTTask.h"


#include "FreeRTOS.h"
#include "MKL46Z4.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "mBT.h"

#include "def.h"


#include "external.h"



void tBTTask(void *pvParameters)
    {

    (void) pvParameters;

    BTStruct gBTStruct;


    sBTTaskQueue = xQueueCreate(1, sizeof(BTStruct)); // Single element queue for latest values acquired
    vQueueAddToRegistry(sBTTaskQueue, "BTStruct");

    sBTTaskEvents = xEventGroupCreate(); // Flag group to notify when new values have been added

    char charVal = 0;
    char buffer[3] = {0};
    int bufferIndex = 0;
    bool isReceiving = false;

    while (TRUE)
	{
	///////////////////////////////////////////////////////
	// Bluetooth //////////////////////////////////////////
	///////////////////////////////////////////////////////

	printf("BT Task\n");

	gBTStruct.command = 0;
	gBTStruct.commandArg = 0;

	while(mBT_ReadData(&charVal))
	    {
	    if(isReceiving)
		{
		buffer[bufferIndex] = charVal;
		bufferIndex++;
		}

	    if (charVal == 0x01) // Start of transmission
		{
		isReceiving = true;
		}
	    else if(charVal == 0x02 || bufferIndex > 2) // End of transmission
		{
		isReceiving = false;
		bufferIndex = 0;

		gBTStruct.command = buffer[0];
		gBTStruct.commandArg = buffer[1];
		}

	}

	xQueueOverwrite(sBTTaskQueue, &gBTStruct);
	xEventGroupSetBits(sBTTaskEvents, kBTTaskNewCommandFlag);

	vTaskDelay(kBTTaskDelay);
	}

    }



/*
 * tHMITask.c
 *
 *  Created on: 22 févr. 2022
 *      Author: luc.froideva
 */

#include "tHMITask.h"


#include "FreeRTOS.h"
#include "MKL46Z4.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"

#include "def.h"

#include "mLcd.h"

#include "external.h"


void tHMITask(void *pvParameters)
    {

    (void) pvParameters;

    InputStruct gInputStruct;

//    char text[32];


    while (TRUE)
	{
//	xEventGroupWaitBits(sInputTaskEvents, kInputTaskNewDataFlag, false, false, portMAX_DELAY);

	// Simply act as a passive observer
	xQueuePeek(sInputTaskQueue, &gInputStruct, portMAX_DELAY);

	// Do stuff ...


//	memset(text, ' ', sizeof text);
//	itoa(gInputStruct.IMUAccZ, text, 10);
//
//	mLcd_WriteEntireDisplay(text);




	printf("Printing Data\n");
	vTaskDelay(kHMITaskDelay);
	}

    }



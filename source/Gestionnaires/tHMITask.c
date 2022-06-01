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
    EnvironmentStruct gEnvironmentStruct;
    PilotStruct gPilotStruct;

    int textSize = 32;
    char text[textSize];


    while (TRUE)
	{
//	xEventGroupWaitBits(sInputTaskEvents, kInputTaskNewDataFlag, false, false, portMAX_DELAY);

	// Simply act as a passive observer
	xQueuePeek(sInputTaskQueue, &gInputStruct, portMAX_DELAY);
	xQueuePeek(sInterpretorTaskQueue, &gEnvironmentStruct, portMAX_DELAY);
	xQueuePeek(sPilotTaskQueue, &gPilotStruct, portMAX_DELAY);

	// Do stuff ...


	for(int i = 0; i < textSize; i++)
	    {
		text[i] = ' ';
	    }



	switch (gPilotStruct.mode) {
	    case kModeAutonomous:
		snprintf(text, textSize, "Mode: %s", "Autonomous");
		break;
	    case kModeManual:
		snprintf(text, textSize, "Mode: %s", "Manual");
		break;
	    case kModeObstacle:
		snprintf(text, textSize, "Mode: %s", "Obstacle");
		break;
	    case kModeShock:
		snprintf(text, textSize, "Mode: %s", "Shock");
		break;

	    default:
		snprintf(text, textSize, "%s", "Error in HMI");
		break;
	}

//	printf("Printing on screen\n");


	mLcd_WriteEntireDisplay(text);




	vTaskDelay(kHMITaskDelay);
	}

    }



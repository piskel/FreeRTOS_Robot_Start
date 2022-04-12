/*
 * tPilotTask.c
 *
 *  Created on: 22 févr. 2022
 *      Author: luc.froideva
 */

#include "tPilotTask.h"

#include "FreeRTOS.h"
#include "MKL46Z4.h"
#include "stdio.h"

#include "def.h"

#include "external.h"

void tPilotTask(void *pvParameters)
    {
    (void) pvParameters;


    PilotStruct gPilotStruct;
    EnvironmentStruct gEnvironmentStruct;



    sPilotTaskQueue = xQueueCreate(1, sizeof(PilotStruct)); // Single element queue for latest values acquired
    sPilotTaskEvents = xEventGroupCreate(); // Flag group to notify when new values have been added




//    float aLineError = 0.0;

    while (TRUE)
	{
	xQueuePeek(sInterpretorTaskQueue, &gEnvironmentStruct, portMAX_DELAY);
	printf("Entering pilot task\n");
//	aLineError = gEnvironmentStruct.linePosition; // If the line position is not 0, we are not centered.

	gPilotStruct.speedLeft = 0.5 - gEnvironmentStruct.linePosition/2;
	gPilotStruct.speedRight = 0.5 + gEnvironmentStruct.linePosition/2;




	xQueueOverwrite(sPilotTaskQueue, &gPilotStruct);
	xEventGroupSetBits(sPilotTaskEvents, kPilotTaskNewDirectionsFlag);

	vTaskDelay(kPilotTaskDelay);
	}

    }

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
#include "math.h"

#include "def.h"

#include "external.h"

void tPilotTask(void *pvParameters)
    {
    (void) pvParameters;


    PilotStruct gPilotStruct;
    EnvironmentStruct gEnvironmentStruct;



    sPilotTaskQueue = xQueueCreate(1, sizeof(PilotStruct)); // Single element queue for latest values acquired
    sPilotTaskEvents = xEventGroupCreate(); // Flag group to notify when new values have been added



    float aCurveA = 0.0f;
    float aCurveB = 0.0f;

    float aFactor = 2.2f;




    while (TRUE)
	{
	xQueuePeek(sInterpretorTaskQueue, &gEnvironmentStruct, portMAX_DELAY);
	printf("Entering pilot task\n");



	if(!gEnvironmentStruct.noLineDetected){

	    //https://www.desmos.com/calculator/wulpir2bto

	    aCurveA = 1-powf(gEnvironmentStruct.linePosition, 2)/aFactor;
	    aCurveB = 1-powf(gEnvironmentStruct.linePosition, 2)*aFactor;


	    gPilotStruct.speedLeft = gEnvironmentStruct.linePosition>0?aCurveB:aCurveA;
	    gPilotStruct.speedRight = gEnvironmentStruct.linePosition>0?aCurveA:aCurveB;


	}
	else
	    {
	    gPilotStruct.speedLeft = 0.0f;
	    gPilotStruct.speedRight = 0.0f;
	    }




	xQueueOverwrite(sPilotTaskQueue, &gPilotStruct);
	xEventGroupSetBits(sPilotTaskEvents, kPilotTaskNewDirectionsFlag);

	vTaskDelay(kPilotTaskDelay);
	}

    }

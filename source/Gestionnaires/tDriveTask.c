/*
 * tDriveTask.c
 *
 *  Created on: 22 févr. 2022
 *      Author: luc.froideva
 */

#include "tDriveTask.h"


#include "FreeRTOS.h"
#include "MKL46Z4.h"
#include "math.h"

#include "def.h"

#include "mIcOc.h"

#include "external.h"


void tDriveTask(void *pvParameters)
    {

    (void) pvParameters;

    PilotStruct gPilotStruct;

    while (TRUE)
	{
	xQueuePeek(sPilotTaskQueue, &gPilotStruct, portMAX_DELAY);
	printf("Drive task\n");

	printf("Speed left : %+f\n", gPilotStruct.speedLeft);
	printf("Speed right : %+f\n", gPilotStruct.speedRight);



	if (gPilotStruct.speedLeft!=0){
	    mIcOc_MoteurGauche(gPilotStruct.speedLeft>0?kEnAvant:kEnArriere, fabs(gPilotStruct.speedLeft));
	}

	if (gPilotStruct.speedRight!=0){
	    mIcOc_MoteurDroit(gPilotStruct.speedRight>0?kEnAvant:kEnArriere, fabs(gPilotStruct.speedRight));
	}


	vTaskDelay(kDriveTaskDelay);
	}

    }



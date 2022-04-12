/*
 * tPilotTask.c
 *
 *  Created on: 22 févr. 2022
 *      Author: luc.froideva
 */

#include "tPilotTask.h"

#include "FreeRTOS.h"
#include "MKL46Z4.h"

#include "def.h"

#include "external.h"

void tPilotTask(void *pvParameters)
    {

    (void) pvParameters;

    while (TRUE)
	{
	// Regulation
	//



	vTaskDelay(kPilotTaskDelay);
	}

    }

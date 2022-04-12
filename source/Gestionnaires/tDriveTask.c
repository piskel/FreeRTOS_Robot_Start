/*
 * tDriveTask.c
 *
 *  Created on: 22 févr. 2022
 *      Author: luc.froideva
 */

#include "tDriveTask.h"


#include "FreeRTOS.h"
#include "MKL46Z4.h"

#include "def.h"

#define kDriveTaskPr 10

#define kDriveTaskDelay ((UInt32)(10/portTICK_RATE_MS)) // Every 10 ms

void tDriveTask(void *pvParameters)
    {

    (void) pvParameters;

    while (TRUE)
	{


	vTaskDelay(kDriveTaskDelay);
	}

    }



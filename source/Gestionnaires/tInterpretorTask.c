/*
 * tInterpretorTask.c
 *
 *  Created on: 22 févr. 2022
 *      Author: luc.froideva
 */

#include "tInterpretorTask.h"

#include "FreeRTOS.h"
#include "MKL46Z4.h"
#include "stdio.h"

#include "def.h"

#include "external.h"


#define LINE_WIDTH 2.5 // cm
#define SENSOR_SPACING 1.25 // cm

void tInterpretorTask(void *pvParameters)
    {
    (void) pvParameters;

    EnvironmentStruct gEnvironmentStruct;
    InputStruct gInputStruct;

    sInterpretorTaskQueue = xQueueCreate(1, sizeof(EnvironmentStruct)); // Single element queue for latest values acquired
    sInterpretorTaskEvents = xEventGroupCreate(); // Flag group to notify when new values have been added


//    float aAccX = 0.0;
//    float aAccY = 0.0;
//    float aAccZ = 0.0;


    // TODO : If less than two consecutive sensors detect a line = error
    // TODO : Determine the angle of the line

    while (TRUE)
	{

	// Waiting for new data
	xEventGroupWaitBits(sInputTaskEvents, kInputTaskNewDataFlag, true, true, portMAX_DELAY);
	printf("Interpreting Data\n");
	// Getting the new data
	xQueuePeek(sInputTaskQueue, &gInputStruct, portMAX_DELAY);


	////////////////////////////////////////////////////////////////////////////////////////
	// Determining if the robot is experiencing a physical shock


//	aAccZ = -((float)gInputStruct.IMUAccZ)/17580*9.81;
//
//	printf("Z Acceleration : %+f\n", aAccZ);





	////////////////////////////////////////////////////////////////////////////////////////
	// We search the center position of the line by making an average of the points detected

	// TODO : Take potential sensor error into account (eg. random pixel detecting a line)
	// TODO : Take into account the width of the line if, say, we detect the line at one of the outermost pixels


	gEnvironmentStruct.noLineDetected = gInputStruct.pixelValues == 0;

	float linePosition = 0; // 0 is the center

	if (!gEnvironmentStruct.noLineDetected)
	    {
	    printf("Line detected\n");
	    float nbPointsDetected = 0.0;
	    float minLineThicknessPossible = 0.0;

	    for (int i = 0; i < 8; i++)
		{
		bool pointDetected = ((gInputStruct.pixelValues>>i)&1)==1;
		nbPointsDetected += (float)pointDetected;
		linePosition += (float)i*pointDetected;
		}
	    linePosition = (linePosition/(nbPointsDetected*7.0)-0.5)*2; // Mulitply by 7*SENSOR_SPACING to get real dimensions and remove the *2 at the end
	    minLineThicknessPossible = nbPointsDetected*SENSOR_SPACING; // To be used to find the angle of the line

	    }

	gEnvironmentStruct.linePosition = linePosition;
	printf("Line center position : %f cm\n", linePosition);



	////////////////////////////////////////////////////////////////////////////////////////
	// Detecting obstacles





	// Sending environment to the queue
	xQueueOverwrite(sInterpretorTaskQueue, &gEnvironmentStruct);
	// Setting the "new environment" flag
	xEventGroupSetBits(sInterpretorTaskEvents, kInterpretorTaskNewEnvironmentFlag);

	vTaskDelay(kInterpretorTaskDelay);
	}

    }

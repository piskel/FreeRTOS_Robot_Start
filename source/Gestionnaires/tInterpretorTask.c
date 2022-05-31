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
#include "math.h"

#include "def.h"

#include "external.h"


#define LINE_WIDTH 2.5 // cm
#define SENSOR_SPACING 1.25 // cm
#define SHOCK_THRESHOLD 7 // m/s^2

void tInterpretorTask(void *pvParameters)
    {
    (void) pvParameters;

    EnvironmentStruct gEnvironmentStruct;
    InputStruct gInputStruct;

    sInterpretorTaskQueue = xQueueCreate(1, sizeof(EnvironmentStruct)); // Single element queue for latest values acquired
    vQueueAddToRegistry(sInterpretorTaskQueue, "EnvironmentStruct");

    sInterpretorTaskEvents = xEventGroupCreate(); // Flag group to notify when new values have been added


    float aAccX = 0.0;
    float aAccY = 0.0;
    float aAccZ = 0.0;


    float frontBuffer[3] = {100};
    int frontBufferIndex = 0;

    float leftBuffer[3] = {100};
    int leftBufferIndex = 0;




    float aLinearAcc = 0.0f;

    // TODO : If less than two consecutive sensors detect a line = error
    // TODO : Determine the angle of the line

    while (TRUE)
	{

	// Waiting for new data
	xEventGroupWaitBits(sInputTaskEvents, kInputTaskNewDataFlag, true, true, portMAX_DELAY);
//	printf("Interpretor task\n");

	// Getting the new data
	xQueuePeek(sInputTaskQueue, &gInputStruct, portMAX_DELAY);


	////////////////////////////////////////////////////////////////////////////////////////
	// Determining if the robot is experiencing a physical shock

	aAccX = -((float)gInputStruct.IMUAccX)/17580*9.81;
	aAccY = -((float)gInputStruct.IMUAccY)/17580*9.81;
	aAccZ = -((float)gInputStruct.IMUAccZ)/17580*9.81;

	// Getting the norm of the 3D acceleration vector (Z is compensated for gravity)
	aLinearAcc = sqrtf(powf(aAccX, 2)+powf(aAccY, 2)+powf(aAccZ+9.81, 2));

	gEnvironmentStruct.shockDetected = aLinearAcc > SHOCK_THRESHOLD;





	////////////////////////////////////////////////////////////////////////////////////////
	// We search the center position of the line by making an average of the points detected

	// TODO : Take potential sensor error into account (eg. random pixel detecting a line)
	// TODO : Take into account the width of the line if, say, we detect the line at one of the outermost pixels


	gEnvironmentStruct.noLineDetected = gInputStruct.pixelValues == 0;

	float linePosition = 0; // 0 is the center

	if (!gEnvironmentStruct.noLineDetected)
	    {
//	    printf("Line detected\n");
	    float nbPointsDetected = 0.0;
//	    float minLineThicknessPossible = 0.0;

	    for (int i = 0; i < 8; i++)
		{
		bool pointDetected = ((gInputStruct.pixelValues>>i)&1)==1;
		nbPointsDetected += (float)pointDetected;
		linePosition += (float)i*pointDetected;
		}
	    linePosition = (linePosition/(nbPointsDetected*7.0)-0.5)*2; // Mulitply by 7*SENSOR_SPACING to get real dimensions and remove the *2 at the end
//	    minLineThicknessPossible = nbPointsDetected*SENSOR_SPACING; // To be used to find the angle of the line

	    }

	gEnvironmentStruct.linePosition = linePosition;
//	printf("Line center position : %f cm\n", linePosition);



	////////////////////////////////////////////////////////////////////////////////////////
	// Detecting obstacles

	frontBuffer[frontBufferIndex] = gInputStruct.frontDistance;
	leftBuffer[leftBufferIndex] = gInputStruct.leftDistance;

	frontBufferIndex = (frontBufferIndex + 1)%3;
	leftBufferIndex = (leftBufferIndex + 1)%3;

	float frontSum = (frontBuffer[0] + frontBuffer[1] + frontBuffer[2])/3;
	float leftSum = (leftBuffer[0] + leftBuffer[1] + leftBuffer[2])/3;

	gEnvironmentStruct.frontObstacleDetected = frontSum < 20;
	gEnvironmentStruct.leftObstacleDetected = leftSum < 40;

	// TODO : Add buffers for these sensors
	gEnvironmentStruct.backObstacleDetected = gInputStruct.backDistance < 20;
	gEnvironmentStruct.rightObstacleDetected = gInputStruct.rightDistance < 20;




//	printf("Front sensor : %+f\n", gInputStruct.frontDistance);
//	printf("Back sensor : %+f\n", gInputStruct.backDistance);
//	printf("Left sensor : %+f\n", gInputStruct.leftDistance);
//	printf("Right sensor : %+f\n", gInputStruct.rightDistance);




	// Sending environment to the queue
	xQueueOverwrite(sInterpretorTaskQueue, &gEnvironmentStruct);
	// Setting the "new environment" flag
	xEventGroupSetBits(sInterpretorTaskEvents, kInterpretorTaskNewEnvironmentFlag);

	vTaskDelay(kInterpretorTaskDelay);
	}

    }

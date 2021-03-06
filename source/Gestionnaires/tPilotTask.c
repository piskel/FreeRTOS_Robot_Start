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

    InputStruct gInputStruct;
//    BTStruct gBTStruct;
    EnvironmentStruct gEnvironmentStruct;



    sPilotTaskQueue = xQueueCreate(1, sizeof(PilotStruct)); // Single element queue for latest values acquired
    vQueueAddToRegistry(sPilotTaskQueue, "PilotStruct");

    sPilotTaskEvents = xEventGroupCreate(); // Flag group to notify when new values have been added

    bool detectedObstaclePreviously = false;


    float aCurveA = 0.0f;
    float aCurveB = 0.0f;

    float aFactor = 2.3f; // TODO: Put in a constant elsewhere

    float speed = 1;

//    bool manualMode = false;
    gPilotStruct.mode = kModeAutonomous;


    EObstacleStateMachine obstacleSM = kObstacleFirstRotation;



    while (TRUE)
	{
	xQueuePeek(sInputTaskQueue, &gInputStruct, portMAX_DELAY);
//	xQueuePeek(sBTTaskQueue, &gBTStruct, portMAX_DELAY);
	xQueuePeek(sInterpretorTaskQueue, &gEnvironmentStruct, portMAX_DELAY);
//	printf("Entering pilot task\n");



	if(gInputStruct.command == kComChangeMode)
	    {

	    switch (gInputStruct.commandArg) {
		case kModeAutonomous:
		    gPilotStruct.mode = kModeAutonomous;
		    break;

		case kModeManual:
		    gPilotStruct.mode = kModeManual;
		    break;

		case kModeShock:
		    gPilotStruct.mode = kModeShock;
		    break;

		default:
		    break;
		}
	    }

	if(gInputStruct.command == kComChangeSpeed)
	    {
	    switch (gInputStruct.commandArg) {
		case 0:
		    speed = 0.3f;
		    break;


		case 1:
		    speed = 0.6f;

		    break;


		case 2:
		    speed = 1.0f;

		    break;
		default:
		    break;
	    }
	}


	gPilotStruct.speedLeft = 0.0f;
	gPilotStruct.speedRight = 0.0f;


	///////////////////////////////////////////////////////
	// Shock Mode

	if(gEnvironmentStruct.shockDetected)
	    {

	    printf("Shock detected");
	    gPilotStruct.mode = kModeShock;

	    }



	///////////////////////////////////////////////////////
	// Autonomous mode

	if(gPilotStruct.mode == kModeAutonomous)
	    {

	    if(gEnvironmentStruct.frontObstacleDetected)
		{
		    // Enter obstacle mode if we detect an obstacle
		    gPilotStruct.mode = kModeObstacle;
		}
	    else if(!gEnvironmentStruct.noLineDetected)
		{

		    // https://www.desmos.com/calculator/wulpir2bto
		    aCurveA = (1-powf(gEnvironmentStruct.linePosition, 2)/aFactor)*speed; //*0.8
		    aCurveB = (1-powf(gEnvironmentStruct.linePosition, 2)*aFactor)*speed; //*0.8

		    gPilotStruct.speedLeft = gEnvironmentStruct.linePosition>0?aCurveB:aCurveA;
		    gPilotStruct.speedRight = gEnvironmentStruct.linePosition>0?aCurveA:aCurveB;
		}

	    }

	///////////////////////////////////////////////////////
	// Manual mode

	if (gPilotStruct.mode == kModeManual)
	    {
	    switch (gInputStruct.command) {
		case kComChangeAngle:;

		    float newAngle = ((float)gInputStruct.commandArg)/255-0.5;

		    printf("Change angle : %f\n", newAngle);

		    aCurveA = 1-powf(newAngle, 2)/aFactor;
		    aCurveB = 1-powf(newAngle, 2)*aFactor;

		    gPilotStruct.speedLeft = newAngle>0?aCurveB:aCurveA;
		    gPilotStruct.speedRight = newAngle>0?aCurveA:aCurveB;
		    break;

		case kComChangeSpeed:
		    // TODO
		    break;

		default:

		    break;
		}
	    }


	///////////////////////////////////////////////////////
	// Obstacle mode

	if(gPilotStruct.mode == kModeObstacle)
	    {
	    switch (obstacleSM) {
		case kObstacleFirstRotation:
		    gPilotStruct.speedLeft = 0.5;
		    gPilotStruct.speedRight = -0.5;

		    if(gEnvironmentStruct.leftObstacleDetected)
			{
			obstacleSM = kObstacleForward;
			}

		    break;

		case kObstacleForward:
		    gPilotStruct.speedLeft = 0.5;
		    gPilotStruct.speedRight = 0.5;


		    if((!gEnvironmentStruct.noLineDetected)&&(gEnvironmentStruct.linePosition < -0.2f))
			{
			obstacleSM = kObstacleFirstRotation;
			gPilotStruct.mode = kModeAutonomous;
			}
		    // If the obstacle is not in sight anymore
		    else if(detectedObstaclePreviously && !gEnvironmentStruct.leftObstacleDetected)
			{
			obstacleSM = kObstacleRotate;
			}

		    break;

		case kObstacleRotate:

		    gPilotStruct.speedLeft = 0.0;
		    gPilotStruct.speedRight = 0.5;



		    if((!gEnvironmentStruct.noLineDetected)&&(gEnvironmentStruct.linePosition < -0.2f))
			{

			obstacleSM = kObstacleFirstRotation;
			gPilotStruct.mode = kModeAutonomous;

			}
		    // If the obstacle is not sight anymore
		    else if(detectedObstaclePreviously && !gEnvironmentStruct.leftObstacleDetected)
			{
			obstacleSM = kObstacleForward;
			}
		    break;

		default:
		    break;
		}
		detectedObstaclePreviously = gEnvironmentStruct.leftObstacleDetected;
	    }


	xQueueOverwrite(sPilotTaskQueue, &gPilotStruct);
	xEventGroupSetBits(sPilotTaskEvents, kPilotTaskNewDirectionsFlag);

	vTaskDelay(kPilotTaskDelay);
	}

    }

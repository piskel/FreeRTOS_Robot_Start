
#include "MKL46Z4.h"
#include "FreeRTOS.h"


#include "task.h"
#include "stdio.h"
#include "lib_str.h"

#include "def.h"

#include "mBT.h"
#include "mIMU.h"
#include "mLeds.h"
#include "mCpu.h"
#include "mLeds.h"
#include "mSwitch.h"
#include "mLcd.h"
#include "mRs232.h"
#include "mIcOc.h"
#include "mMeteo.h"
#include "mLine.h"


#include "external.h"

#include "tInputTask.h"
#include "tInterpretorTask.h"
#include "tPilotTask.h"
#include "tDriveTask.h"
#include "tHMITask.h"



#define kAppStartTaskPr 3

#define kStackSize 	256


void AppStartTask(void *pvParameters);


void AppStartTask(void *pvParameters)
    {
    BaseType_t aErr;

    (void) pvParameters;

    mCpu_Setup();
    mLeds_Setup();
    mIcOc_Setup();
    mLcd_Setup();
    mRs232_Setup();
    mSwitch_Setup();
    mLine_Setup();
    mIMU_Setup();

    // TODO : Check why in a while loop
    while (TRUE)
	{
	mIcOc_Open();
	mLcd_Open();
	mRs232_Open();
	mLine_Open();
	mIMU_Open();

	aErr = xTaskCreate(
		tInputTask,
		(const char*) "Input task",
		kStackSize,
		(void*) NULL,
		kInputTaskPr,
		(xTaskHandle*) NULL);

	aErr = xTaskCreate(
		tInterpretorTask,
		(const char*) "Interpretor task",
		kStackSize,
		(void*) NULL,
		kInterpretorTaskPr,
		(xTaskHandle*) NULL);


	aErr = xTaskCreate(
		tPilotTask,
		(const char*) "Pilot task",
		kStackSize,
		(void*) NULL,
		kPilotTaskPr,
		(xTaskHandle*) NULL);


	aErr = xTaskCreate(
		tDriveTask,
		(const char*) "Drive task",
		kStackSize,
		(void*) NULL,
		kDriveTaskPr,
		(xTaskHandle*) NULL);


	aErr = xTaskCreate(
		tHMITask,
		(const char*) "HMI task",
		kStackSize,
		(void*) NULL,
		kHMITaskPr,
		(xTaskHandle*) NULL);


	if (aErr != pdPASS)
	    {
	    for (;;)
		;
	    }

	vTaskDelete(NULL);
	}
    }

int main(void)
    {
    BaseType_t aErr;

    aErr = xTaskCreate(
	    AppStartTask,
	    (const char*) "Start task",
	    kStackSize,
	    (void*) NULL,
	    kAppStartTaskPr,
	    (xTaskHandle*) NULL);

    if (aErr != pdPASS)
	{
	for (;;)
	    ;
	}

    vTaskStartScheduler();

    for (;;)
	;

    }



#ifndef __EXTERNAL_H__
#define __EXTERNAL_H__

#include "FreeRTOS.h"
#include "event_groups.h"
#include "queue.h"
#include "def.h"

///////////////////////////////////////////////////////////////
// INPUT TASK /////////////////////////////////////////////////
///////////////////////////////////////////////////////////////

// CONSTANTS

#define kInputTaskPr 10
#define kInputTaskDelay ((UInt32)(10/portTICK_RATE_MS)) // Every 10ms
#define kInputTaskNewDataFlag 0x01


// GLOBAL VARIABLES

typedef struct
    {
    UInt8 switchValues;
    UInt8 pixelValues;

    float frontDistance;
    float backDistance;
    float leftDistance;
    float rightDistance;

    Int16 IMUAccX;
    Int16 IMUAccY;
    Int16 IMUAccZ;
    Int16 IMUGyroX;
    Int16 IMUGyroY;
    Int16 IMUGyroZ;

    bool getPixelValuesError;
    }InputStruct;


extern QueueHandle_t sInputTaskQueue;
extern EventGroupHandle_t sInputTaskEvents;




///////////////////////////////////////////////////////////////
// INTERPRETOR TASK ///////////////////////////////////////////
///////////////////////////////////////////////////////////////

// CONSTANTS

#define kInterpretorTaskPr 9
#define kInterpretorTaskDelay ((UInt32)(10/portTICK_RATE_MS)) // Every 10ms
#define kInterpretorTaskNewEnvironmentFlag 0x01



// GLOBAL VARIABLES

typedef struct
    {;
    bool obstacleDetected;
    bool noLineDetected;

    float linePosition;
    float lineAngle;

    }EnvironmentStruct;


extern QueueHandle_t sInterpretorTaskQueue;
extern EventGroupHandle_t sInterpretorTaskEvents;


///////////////////////////////////////////////////////////////
// PILOT TASK /////////////////////////////////////////////////
///////////////////////////////////////////////////////////////

#define kPilotTaskPr 8
#define kPilotTaskDelay ((UInt32)(10/portTICK_RATE_MS)) // Every 10ms
#define kPilotTaskNewDirectionsFlag 0x01

typedef struct
    {
    float speedLeft; // from -1 to 1
    float speedRight;

    }PilotStruct;


extern QueueHandle_t sPilotTaskQueue;
extern EventGroupHandle_t sPilotTaskEvents;



///////////////////////////////////////////////////////////////
// DRIVE TASK /////////////////////////////////////////////////
///////////////////////////////////////////////////////////////


#define kDriveTaskPr 7
#define kDriveTaskDelay ((UInt32)(10/portTICK_RATE_MS)) // Every 10 ms


///////////////////////////////////////////////////////////////
// HMI TASK ///////////////////////////////////////////////////
///////////////////////////////////////////////////////////////

// CONSTANTS

#define kHMITaskPr 3
#define kHMITaskDelay ((UInt32)(10/portTICK_RATE_MS)) // Every 10ms


#endif

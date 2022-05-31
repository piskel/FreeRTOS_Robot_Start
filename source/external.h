
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

typedef enum
    {
    kComNoOperation = 0,
    kComChangeMode = 1, // 0: Autonomous, 1: Manual, 2: Obstacle, 3: Shock
    kComChangeAngle = 2,
    kComChangeSpeed = 3
    }ECommand;


typedef struct
    {

//    ECommand command;
//    char commandArg;

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
    {
    bool frontObstacleDetected;
    bool backObstacleDetected;
    bool leftObstacleDetected;
    bool rightObstacleDetected;

    bool noLineDetected;
    bool shockDetected;

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

typedef enum
    {
    kModeAutonomous = 0,
    kModeManual = 1,
    kModeObstacle = 2,
    kModeShock = 3
    }EPilotMode;

typedef enum
    {
	kObstacleFirstRotation,
	kObstacleForward,
	kObstacleRotate
    }EObstacleStateMachine;

typedef struct
    {

    int mode; // 0: automatic, 1: manual, 2: obstacle avoiding

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

#define kHMITaskPr 6
#define kHMITaskDelay ((UInt32)(500/portTICK_RATE_MS)) // Every 10ms



///////////////////////////////////////////////////////////////
// BT TASK ////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////

// CONSTANTS

#define kBTTaskPr 5
#define kBTTaskDelay ((UInt32)(100/portTICK_RATE_MS)) // Every 10ms
#define kBTTaskNewCommandFlag 0x01

typedef struct
    {

    ECommand command;
    char commandArg;
    } BTStruct;


extern QueueHandle_t sBTTaskQueue;
extern EventGroupHandle_t sBTTaskEvents;


#endif

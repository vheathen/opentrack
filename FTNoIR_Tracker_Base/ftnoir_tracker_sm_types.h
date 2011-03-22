//
// Definitions for the Shared Memory to send the data to FaceTrackNoIR
//
static const char* SM_MM_DATA = "SM_SharedMem";
static const char* SM_FACEAPI = "SM_FaceAPI";
static const char* SM_MUTEX = "SM_Mutex";

#include "sm_api.h"

struct TFaceData {
	int DataID;
	smEngineHeadPoseData new_pose;
};
typedef TFaceData * PFaceData;

struct SMMemMap {
	int command;					// Command from FaceTrackNoIR
	int status;						// Status from faceAPI
	TFaceData data;
	HANDLE handle;
};
typedef SMMemMap * PSMMemMap;

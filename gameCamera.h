#ifndef GAME_CAMERA
#define GAME_CAMERA

#include "gvector.h"
#include <stdbool.h>

#define CAMERA_POINT_FREE 0
#define CAMERA_POINT_AUTO 1

struct CameraPoint{
    int x;
    int y;
    int cameraPointType;
};
typedef struct CameraPoint CameraPoint;


struct CameraManager{
    float cameraX;
    float cameraY;
    Vector* cameraPoints;
    CameraPoint* currentPoint;
    CameraPoint* nextPoint;
    float currentProgress;
    int currentPointIndex;
    bool reachedEnd;
    float cameraSpeed;
};
typedef struct CameraManager CameraManager;


CameraManager* getCameraManager();

void updateCameraManager(CameraManager* manager);
void updateGameCameraPosition(CameraManager* manager, float x, float y);
void addCameraMarker(CameraManager* manager, int x, int y, int type);
void unloadCameraManager(CameraManager* manager);



#endif
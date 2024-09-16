#ifndef GAME_CAMERA
#define GAME_CAMERA

#include "gcollections.h"
#include <stdbool.h>

#define CAMERA_POINT_FREE 0
#define CAMERA_POINT_AUTO 1
#define CAMERA_SPECIAL 2

struct CameraPoint{
    int x;
    int y;
    int cameraPointType;
    int extraIndex;
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
    int markerCameraType;
    char startTimer;
    float cameraSpeedMultiplier;

};
typedef struct CameraManager CameraManager;


CameraManager* getCameraManager();

void updateCameraManager(CameraManager* manager);
void updateGameCameraPosition(CameraManager* manager, float x, float y);
void addCameraMarker(CameraManager* manager, int x, int y, int type, int extraEffect);
void clearCameraMarkers(CameraManager* manager);
void unloadCameraManager(CameraManager* manager);
bool isOnScreen(CameraManager* manager, int x, int y, int w, int h);
void resetCameraManager(CameraManager* manager);
void setScrollSpeed(CameraManager* manager, float speed);



#endif
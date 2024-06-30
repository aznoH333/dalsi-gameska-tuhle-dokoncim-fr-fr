#include "gameCamera.h"
#include "gframework.h"
#include "gutil.h"
#include "gvector.h"
#include <stdlib.h>

CameraManager* initCameraManager(){
    CameraManager* out = malloc(sizeof(CameraManager));

    out->cameraX = 0;
    out->cameraY = 0;
    out->cameraPoints = initVector();
    out->currentPoint = 0;
    out->nextPoint = 0;
    out->currentProgress = 0;
    out->currentPointIndex = 0;
    out->reachedEnd = false;

    return out;
}

CameraManager* instance;
CameraManager* getCameraManager(){
    if (!instance) {
        instance = initCameraManager();
    }
    return instance;
}

void addCameraMarker(CameraManager* manager, int x, int y, int type){
    CameraPoint* point = malloc(sizeof(CameraPoint));

    point->x = x;
    point->y = y;
    point->cameraPointType = type;

    gLog(LOG_INF, "Added camera marker x: %d y: %d type: %d", x, y, type);

    if (manager->cameraPoints.elementCount == 0){
        manager->currentPoint = point;

    }else if (manager->cameraPoints.elementCount == 1){
        manager->nextPoint = point;
    }

    vectorPush(&manager->cameraPoints, point);
}

void unloadCameraManager(CameraManager* manager){
    vectorFree(&manager->cameraPoints);
}


void updateCameraManager(CameraManager* manager){
    
    {// calculate path to next
        manager->cameraX = (manager->currentProgress * (manager->nextPoint->x - manager->currentPoint->x)) + manager->currentPoint->x;
        manager->cameraY = (manager->currentProgress * (manager->nextPoint->y - manager->currentPoint->y)) + manager->currentPoint->y;
    }



    {// check progress
        if (manager->currentProgress >= 1){

            if (manager->currentPointIndex != manager->cameraPoints.elementCount - 2){
                manager->currentProgress = 0;
                manager->currentPointIndex++;

                manager->currentPoint = vectorGet(&manager->cameraPoints, manager->currentPointIndex);
                manager->nextPoint = vectorGet(&manager->cameraPoints, manager->currentPointIndex + 1);
            }else {
                manager->reachedEnd = true;
                manager->currentPoint = manager->nextPoint;
                manager->currentProgress = 0;
            }
            gLog(LOG_INF, "progress %d current x %d current y %d", manager->currentPointIndex, manager->currentPoint->x, manager->currentPoint->y);
        }
    }

    // temporary cheaty camera movement
    if (IsKeyDown(KEY_F) && manager->reachedEnd == false){
        manager->currentProgress += 0.01;
    }


    setCameraPos((Vector2){manager->cameraX, manager->cameraY});
}

void updateGameCameraPosition(CameraManager* manager, float x, float y){
    
}
#include "gameCamera.h"
#include "gframework.h"
#include "gutil.h"
#include "gvector.h"
#include <stdlib.h>
#include "entities.h"
#include "gameplay.h"

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
    out->cameraSpeed = 0;

    return out;
}

CameraManager* instance;
CameraManager* getCameraManager(){
    if (!instance) {
        instance = initCameraManager();
    }
    return instance;
}

void calculateSegmentSpeed(CameraManager* manager){
    manager->cameraSpeed = 1 / pythagoras(manager->currentPoint->x, manager->currentPoint->y, manager->nextPoint->x, manager->nextPoint->y);
}

void addCameraMarker(CameraManager* manager, int x, int y, int type){
    CameraPoint* point = malloc(sizeof(CameraPoint));

    point->x = x;
    point->y = y;
    point->cameraPointType = type;

    gLog(LOG_INF, "Added camera marker x: %d y: %d type: %d", x, y, type);

    if (manager->cameraPoints->elementCount == 0){
        manager->currentPoint = point;

    }else if (manager->cameraPoints->elementCount == 1){
        manager->nextPoint = point;
        calculateSegmentSpeed(manager);

    }

    vectorPush(manager->cameraPoints, point);
}

void clearCameraMarkers(CameraManager* manager){
    vectorClear(manager->cameraPoints);
    manager->currentProgress = 0;
}


void unloadCameraManager(CameraManager* manager){
    vectorFree(manager->cameraPoints);
}


void updateCameraManager(CameraManager* manager){
    
    {// activate entity markers
        Gameplay* g = getGameplay();
        
        for (int i = 0; i < g->level->entityeMarkers->elementCount; i++){
            EntityMarker* marker = vectorGet(g->level->entityeMarkers, i);

            if (checkBoxCollisions(manager->cameraX, manager->cameraY, SCREEN_WIDTH / DEFAULT_CAMERA_ZOOM, SCREEN_HEIGHT / DEFAULT_CAMERA_ZOOM, marker->x * 16, marker->y * 16, 16, 16)){
                activateEntityMarker(marker);
            }
        }
    }
    
    
    
    
    {// calculate path to next
        manager->cameraX = (manager->currentProgress * (manager->nextPoint->x - manager->currentPoint->x)) + manager->currentPoint->x;
        manager->cameraY = (manager->currentProgress * (manager->nextPoint->y - manager->currentPoint->y)) + manager->currentPoint->y;
    }



    {// check progress
        if (manager->currentProgress >= 1){

            if (manager->currentPointIndex != manager->cameraPoints->elementCount - 2){
                manager->currentProgress = 0;
                manager->currentPointIndex++;

                manager->currentPoint = vectorGet(manager->cameraPoints, manager->currentPointIndex);
                manager->nextPoint = vectorGet(manager->cameraPoints, manager->currentPointIndex + 1);
                calculateSegmentSpeed(manager);
                gLog(LOG_INF, "camera progress %d current x %d current y %d", manager->currentPointIndex, manager->currentPoint->x, manager->currentPoint->y);

            }else {
                manager->reachedEnd = true;
                manager->currentPoint = manager->nextPoint;
                manager->currentProgress = 0;
                gLog(LOG_INF, "camera reached end");
            }
        }
    }



    setCameraPos((Vector2){manager->cameraX, manager->cameraY});
}

void updateGameCameraPosition(CameraManager* manager, float x, float y){
    
    if (manager->currentPoint->cameraPointType != CAMERA_POINT_FREE || manager->reachedEnd){
        return;
    }


    x -= SCREEN_WIDTH / DEFAULT_CAMERA_ZOOM / 2.0f;
    y -= SCREEN_HEIGHT / DEFAULT_CAMERA_ZOOM / 2.0f;
    
    
    float xProgress = (x - manager->currentPoint->x) / (manager->nextPoint->x - manager->currentPoint->x);

    if (xProgress > manager->currentProgress){
        manager->currentProgress = xProgress;
    }

    
    /*
    if (manager->currentPoint->y == manager->nextPoint->y){
        return;
    }
    bool isGoingUp = manager->currentPoint->y > manager->nextPoint->y;
    


    if (isGoingUp && y < manager->cameraY){
        manager->currentProgress = (y - manager->currentPoint->y) / (manager->nextPoint->y - manager->currentPoint->y);
    }
    else if (!isGoingUp && y > manager->cameraY){
        manager->currentProgress = (y - manager->currentPoint->y) / (manager->nextPoint->y - manager->currentPoint->y);
    }*/

}
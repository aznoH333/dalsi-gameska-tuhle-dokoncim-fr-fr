#include "gameplay.h"
#include "enemy.h"
#include "entities.h"
#include "gameCamera.h"
#include "level.h"
#include "player.h"
#include "gameprogress.h"


Gameplay* initGameplay(){
    Gameplay* output = malloc(sizeof(Gameplay));
    output->level = 0;
    output->playerX = 0.0f;
    output->playerY = 0.0f;
    output->hasLoadedLevel = false;
    return output;    
}


Gameplay* gameplayInstance;
Gameplay* getGameplay(){
    if (gameplayInstance == 0) {
        gameplayInstance = initGameplay();
    }
    return gameplayInstance;
}

void startLevel(Gameplay* g, const char* levelPath){
    // level load
    {
        if (g->hasLoadedLevel){
            unloadLevel(g->level);
        }
            
        
        g->level = loadLevel(levelPath);
        g->hasLoadedLevel = true;
    }

    // setup camera markers
    {
        CameraManager* m = getCameraManager();
        int markerCount = 0;

        for (int x = 0; x < g->level->width; x++){
            for (int y = 0; y < g->level->height; y++){
                for (int i = 0; i < g->level->entityeMarkers->elementCount; i++){
                    EntityMarker* marker = vectorGet(g->level->entityeMarkers, i);

                    if (marker->x == x && marker->y == y && (marker->id == 2 || marker->id == 3)){
                        addCameraMarker(m, x * 16, y * 16, marker->id - 2);
                        markerCount++;
                    }
                }
            }
        }

        if (markerCount < 2){
            gLog(LOG_WAR, "Too few camera markers in %s", levelPath);
        }
    }


    // find player
    {
        EntityMarker* playerMarker;
        for (int i = 0; i < g->level->entityeMarkers->elementCount; i++){
            playerMarker = vectorGet(g->level->entityeMarkers, i);
            
            if (playerMarker->id == 0){
                break;
            }
        }
        addEntity(getEntityManager(), initPlayer(playerMarker->x * 16, playerMarker->y * 16));
    }


    // play music
    {
        playMusic(0);
    }
}

void setPlayerCoordinates(Gameplay* gameplay, float x, float y){
    gameplay->playerX = x;
    gameplay->playerY = y;
}


EntityMarker* getCollidingMarker(Gameplay* g, Entity* entity){

    for (int i = 0; i < g->level->entityeMarkers->elementCount; i++){
        EntityMarker* marker = vectorGet(g->level->entityeMarkers, i);

        if (checkBoxCollisions(marker->x * 16, marker->y * 16, 16, 16, entity->x, entity->y, entity->w, entity->h)){
            return marker;
        }
    }

    return 0;
}


void unloadGameplay(Gameplay* g){
    if (g->hasLoadedLevel){
        unloadLevel(g->level);
        clearCameraMarkers(getCameraManager());
        removeAllEntities(getEntityManager());
        g->hasLoadedLevel = false;
    }
}

void disposeGameplay(Gameplay* g){
    unloadGameplay(g);
    free(g);
}



void updateGameplay(Gameplay* g){
    drawLevel(g->level, LEVEL_DRAW_GAME);
    displayPlayerUi();
    updateEntityManager(getEntityManager());
    updateCameraManager(getCameraManager());
}

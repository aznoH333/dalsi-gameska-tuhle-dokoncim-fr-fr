#include "gameplay.h"
#include "enemy.h"
#include "entities.h"
#include "gameCamera.h"
#include "level.h"
#include "player.h"



Gameplay* initGameplay(){
    Gameplay* output = malloc(sizeof(Gameplay));
    output->level = 0;
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
                for (int i = 0; i < g->level->entityeMarkers.elementCount; i++){
                    EntityMarker* marker = vectorGet(&g->level->entityeMarkers, i);

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
        for (int i = 0; i < g->level->entityeMarkers.elementCount; i++){
            playerMarker = vectorGet(&g->level->entityeMarkers, i);
            
            if (playerMarker->id == 0){
                break;
            }
        }
        addEntity(getEntityManager(), initPlayer(playerMarker->x * 16, playerMarker->y * 16));
    }
}


void unloadGameplay(Gameplay* g){
    if (g->hasLoadedLevel){
        unloadLevel(g->level);
    }
    free(g);
}



void updateGameplay(Gameplay* g){
    drawLevel(g->level, LEVEL_DRAW_GAME);
    updateEntityManager(getEntityManager());
    updateCameraManager(getCameraManager());
}

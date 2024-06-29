#include "gameplay.h"
#include "enemy.h"
#include "entities.h"
#include "level.h"
#include "player.h"



Gameplay* initGameplay(){
    Gameplay* output = malloc(sizeof(Gameplay));
    output->level = 0;
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
        if (g->level != 0){
            unloadLevel(g->level);
        }
            
        
        g->level = loadLevel(levelPath);
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

    // temporary spawn all enemies
    {
        for (int i = 0; i < g->level->entityeMarkers.elementCount; i++){
            EntityMarker* marker = vectorGet(&g->level->entityeMarkers, i);

            if (marker->id == 9){
                addEntity(getEntityManager(), initEnemy(marker->x * 16, marker->y * 16, ENEMY_GREY_LIZARD));
            }
        }
    }
}


void unloadGameplay(Gameplay* g){
    unloadLevel(g->level);
    free(g);
}



void updateGameplay(Gameplay* g){
    drawLevel(g->level, LEVEL_DRAW_GAME);
    updateEntityManager(getEntityManager());
}

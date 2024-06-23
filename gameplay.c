#include "gameplay.h"

Gameplay* startGameplay(const char* levelPath){
    Gameplay* output = malloc(sizeof(Gameplay));
    output->level = loadLevel(levelPath);

    // find player
    EntityMarker* playerMarker;
    for (int i = 0; i < output->level->entityeMarkers.elementCount; i++){
        playerMarker = vectorGet(&output->level->entityeMarkers, i);
        
        if (playerMarker->id == 0){
            break;
        }
    }
    
    return output;
}


void stopGameplay(Gameplay* g){
    unloadLevel(g->level);
    free(g);
}



void updateGameplay(Gameplay* g){
    drawLevel(g->level, LEVEL_DRAW_GAME);
}

#include "gameplay.h"



Gameplay* startGameplay(const char* levelPath){
    Gameplay* output = malloc(sizeof(Gameplay));
    output->level = loadLevel(levelPath);

    
    return output;
}


void stopGameplay(Gameplay* g){
    unloadLevel(g->level);
    free(g);
}



void updateGameplay(Gameplay* g){
    drawLevel(g->level, LEVEL_DRAW_GAME);
}

#ifndef GAMEPLAY
#define GAMEPLAY

#include "level.c"
#include "entities.c"
#include "gframework.c"

struct Gameplay{
    Level* level;
};
typedef struct Gameplay Gameplay;

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
    drawLevel(g->level);
}

#endif
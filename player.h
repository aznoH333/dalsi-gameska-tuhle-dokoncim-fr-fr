#ifndef PLAYER
#define PLAYER

#include "entities.h"

struct Player{

};
typedef struct Player Player;




Entity* initPlayer(int x, int y);
void playerUpdate(Entity* this);
void playerOnCollide(Entity* this, Entity* other);
void playerOnDestroy(Entity* this);
void playerClean(Entity* this);

#endif
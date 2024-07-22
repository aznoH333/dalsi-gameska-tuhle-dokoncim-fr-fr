#ifndef ENEMY
#define ENEMY

#include "entities.h"
#include "extraEntityData.h"



struct Enemy{
    float moveSpeed;
    bool flipDirection;
    int enemyType;
    int health;
    int hurtTimer;
    int animationTimer;
    int animationFrameDuration;
    int animationFrame;
    int aiType;
    int baseSprite;
    int bodyType;

    float yVelocity;
    float xVelocity;
    float gravity;

    bool isTouchingGround;


};
typedef struct Enemy Enemy;

// enemy type
#define ENEMY_GREY_LIZARD 0
#define ENEMY_GREEN_LIZARD 1
#define ENEMY_PINK_LIZARD 2
#define ENEMY_GREY_ROBOT 3
#define ENEMY_GREEN_ROBOT 4
#define ENEMY_GREEN_SOLDIER 5
#define ENEMY_GREY_SOLDIER 6
#define ENEMY_BLUE_SOLDIER 7
#define ENEMY_RED_SOLDIER 8

// ai
#define AI_BASIC 0
#define AI_GRUNT 1


// body type
#define BODY_FLESH 0
#define BODY_ROBOT 1



Entity* initEnemy(int x, int y, int type);
void enemyUpdate(Entity* this);
void enemyOnCollide(Entity* this, Entity* other);
void enemyOnDestroy(Entity* this);
void enemyClean(Entity* this);

#endif
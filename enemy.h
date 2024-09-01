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
#define ENEMY_LIZARD_RED 0
#define ENEMY_LIZARD_BLUE 1
#define ENEMY_ROBOT_RED 2
#define ENEMY_ROBOT_BLUE 3
#define ENEMY_SOLDIER_RED 4
#define ENEMY_SOLDIER_BLUE 5
#define ENEMY_FLY_RED 6
#define ENEMY_FLY_BLUE 7
#define ENEMY_LARGE_FLY_RED 8
#define ENEMY_LARGE_FLY_BLUE 9
#define ENEMY_SQUID_RED 10
#define ENEMY_SQUID_BLUE 11
#define ENEMY_BOSS_RED 12
#define ENEMY_BOSS_BLUE 13

// ai
#define AI_BASIC 0
#define AI_GRUNT 1
#define AI_FLY 2

// body type
#define BODY_FLESH 0
#define BODY_ROBOT 1
#define BODY_LARGE 2


Entity* initEnemyDirectional(int x, int y, int type, bool flipDirection);
Entity* initEnemy(int x, int y, int type);
void enemyUpdate(Entity* this);
void enemyOnCollide(Entity* this, Entity* other);
void enemyOnDestroy(Entity* this);
void enemyClean(Entity* this);

#endif
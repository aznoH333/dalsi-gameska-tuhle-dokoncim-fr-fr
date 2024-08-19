#ifndef CANNON_TURRET
#define CANNON_TURRET

#include "entities.h"

#define DIRECTION_LEFT 0
#define DIRECTION_RIGHT 1
#define DIRECTION_UP 2
#define DIRECTION_DOWN 3
#define DEFAULT_CANNON_COOLDOWN 200
#define CANNON_PROJECTILE_SPEED 1.3f

struct CannonTurret{
    unsigned char cooldown;
    char direction;
};
typedef struct CannonTurret CannonTurret;


Entity* initCannon(int x, int y, char direction);
void cannonUpdate(Entity* this);
void cannonOnCollide(Entity* this, Entity* other);
void cannonOnDestroy(Entity* this);
void cannonClean(Entity* this);

#endif
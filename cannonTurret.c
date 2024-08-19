#include "cannonTurret.h"
#include "gframework.h"
#include "spritedata.h"
#include "bullet.h"

Entity* initCannon(int x, int y, char direction){
    CannonTurret* data = malloc(sizeof(CannonTurret));
    data->cooldown = DEFAULT_CANNON_COOLDOWN;
    data->direction = direction;

    
    Entity* this = initEntity(x, y, 0, 0, ENTITY_OTHER, data, &cannonUpdate, &cannonOnCollide, &cannonOnDestroy, &cannonClean);

    return this;
}


void cannonUpdate(Entity* this){
    CannonTurret* data = this->data;

    // cooldown
    data->cooldown--;
    if (data->cooldown == 0){
        data->cooldown = DEFAULT_CANNON_COOLDOWN;

        // shoot
        float xV = 0;
        float yV = 0;

        if (data->direction <= DIRECTION_RIGHT){
            xV = boolToSign(data->direction == DIRECTION_RIGHT) * CANNON_PROJECTILE_SPEED;
        }else {
            yV = boolToSign(data->direction == DIRECTION_DOWN) * CANNON_PROJECTILE_SPEED;
        }

        addEntity(getEntityManager(), initBullet(this->x, this->y, xV, yV, SPRITE_START_ENTITIES + 14, ENTITY_ENEMY, BULLET_FLAG_PHASING));
    }


    // draw
    draw(data->direction + SPRITE_START_ENTITIES + 10, this->x, this->y, LAYER_OBJECTS);
}


void cannonOnCollide(Entity* this, Entity* other){}
void cannonOnDestroy(Entity* this){}
void cannonClean(Entity* this){}
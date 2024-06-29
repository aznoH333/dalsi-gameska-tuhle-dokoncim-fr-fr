#include "enemy.h"
#include "entities.h"
#include "gframework.h"
#include "bullet.h"

void initEnemyBasedOnType(Enemy* enemy, Entity* entity, int enemyType){
    
    enemy->enemyType = enemyType;
    enemy->flipDirection = 0;
    enemy->hurtTimer = 0;

    switch (enemyType) {
        case ENEMY_GREY_LIZARD:
            enemy->health = 20;
            enemy->moveSpeed = 1.0f;
            entity->w = 16;
            entity->h = 16;
            break;
        default:
            gLog(LOG_ERR,"Unknown enemy type %d", enemyType);
            break;
    }

}



Entity* initEnemy(int x, int y, int type){
    Enemy* enemy = malloc(sizeof(Enemy));
    Entity* out = initEntity(x, y, 16, 16, ENTITY_ENEMY, enemy, &enemyUpdate, &enemyOnCollide, &enemyOnDestroy, &enemyClean);

    initEnemyBasedOnType(enemy, out, type);

    return out;
}

const int HURT_TIMER_MAX = 10;
void enemyUpdate(Entity* this){
    Enemy* data = this->data;
    
    
    
    
    {// draw
        // hurt timer
        data->hurtTimer -= data->hurtTimer > 0;
        
        // color and scale
        float hurtPercentage = (float) data->hurtTimer / HURT_TIMER_MAX;
        float scaleMultiplier = (hurtPercentage * 0.3f) + 1.0f;
        float healthColor =  (1 - (hurtPercentage * 0.4));
        Color c = {255, 255 * healthColor, 255 * healthColor, 255};


        // draw
        drawFSC(87, this->x, this->y, data->flipDirection, scaleMultiplier, c, LAYER_OBJECTS);
    }
}


void takeDamage(Entity* this, int damage){
    Enemy* data = this->data;

    data->health -= damage;
    data->hurtTimer = HURT_TIMER_MAX;

    if (data->health <= 0){
        this->shouldDestroy = true;
    }
}


void enemyOnCollide(Entity* this, Entity* other){
    if (other->identifier == ENTITY_PLAYER_PROJECTILE){
        
        Bullet* otherData = other->data;
        takeDamage(this, otherData->damage);
        other->shouldDestroy = true;

    }
}
void enemyOnDestroy(Entity* this){

}
void enemyClean(Entity* this){
    
}
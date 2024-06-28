#include "enemy.h"
#include "gframework.h"

void initEnemyBasedOnType(Enemy* enemy, Entity* entity, int enemyType){
    
    enemy->enemyType = enemyType;

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

void enemyUpdate(Entity* this){
    Enemy* data = this->data;
    
    {// draw
        drawF(87, this->x, this->y, 0, LAYER_OBJECTS);
    }
}
void enemyOnCollide(Entity* this, Entity* other){

}
void enemyOnDestroy(Entity* this){

}
void enemyClean(Entity* this){
    
}
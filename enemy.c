#include "enemy.h"
#include "entities.h"
#include "gameplay.h"
#include "gframework.h"
#include "bullet.h"
#include "gutil.h"
#include "level.h"
#include "particleEffect.h"
#include "spritedata.h"
#include "extraGraphics.h"
#include "gameprogress.h"




void initBasicEnemy(Enemy* enemy){
    enemy->health = 20;
    enemy->moveSpeed = 0.6f;
    enemy->animationFrameDuration = 10;
    enemy->aiType = AI_BASIC;
}

void initSoldierEnemy(Enemy* enemy){
    enemy->health = 25;
    enemy->moveSpeed = 0.6f;
    enemy->animationFrameDuration = 8;
    enemy->aiType = AI_GRUNT;
    
}
void gunnerUpdate(Entity* this);
void initSoldierData(Entity* entity, int fireRate){
    ExtraGunnerData* g = malloc(sizeof(ExtraGunnerData));
    g->fireRate = fireRate;
    g->cooldown = fireRate;
    entity->updateFunction = &gunnerUpdate;

    entity->extraIndex = allocateExtraEntityData(getEntityManager(), g);
}

void flyUpdate(Entity* this);
void initFlyData(Entity* this){
    ExtraFlyData* f = malloc(sizeof(ExtraFlyData));
    f->targetHeight = getGameplay()->playerY + getRandomIntR(-20, 20);
    f->movingDown = getGameplay()->playerY > this->y;
    this->updateFunction = &flyUpdate;
    this->extraIndex = allocateExtraEntityData(getEntityManager(), f);
}

void initEnemyBasedOnType(Enemy* enemy, Entity* entity, int enemyType){
    
    enemy->enemyType = enemyType;
    enemy->flipDirection = 0;
    enemy->hurtTimer = 0;
    enemy->animationFrame = 0;
    enemy->gravity = 0.1f;
    enemy->xVelocity = 0;
    enemy->yVelocity = 0;
    enemy->isTouchingGround = true;
    entity->w = 16;
    entity->h = 16;
    enemy->bodyType = BODY_FLESH;

    switch (enemyType) {
        case ENEMY_GREY_LIZARD:
            initBasicEnemy(enemy);
            enemy->baseSprite = SPRITE_START_ENTITIES;
            break;

        case ENEMY_GREEN_LIZARD:
            initBasicEnemy(enemy);
            enemy->health = 23;
            enemy->moveSpeed = 1.0f;
            enemy->baseSprite = SPRITE_START_ENTITIES + 2;
            break;

        case ENEMY_PINK_LIZARD:
            initBasicEnemy(enemy);
            enemy->health = 26;
            enemy->baseSprite = SPRITE_START_ENTITIES + 4;
            break;

        case ENEMY_GREY_ROBOT:
            initBasicEnemy(enemy);
            enemy->health = 75;
            enemy->baseSprite = SPRITE_START_ENTITIES + 6;
            enemy->bodyType = BODY_ROBOT;
            break;  

        case ENEMY_GREEN_ROBOT:  
            initBasicEnemy(enemy);
            enemy->health = 65;
            enemy->moveSpeed = 1.0f;
            enemy->baseSprite = SPRITE_START_ENTITIES + 8;
            enemy->bodyType = BODY_ROBOT;
            break;  


        case ENEMY_GREEN_SOLDIER:
            initSoldierEnemy(enemy);
            enemy->baseSprite = SPRITE_START_ENTITIES + 27;
            initSoldierData(entity, 85);

            break;
        
        case ENEMY_GREY_SOLDIER:
            initSoldierEnemy(enemy);
            enemy->moveSpeed = 0.5f;
            enemy->baseSprite = SPRITE_START_ENTITIES + 29;
            enemy->health = 50;
            initSoldierData(entity, 120);
            break;

        case ENEMY_BLUE_SOLDIER:
            initSoldierEnemy(enemy);
            enemy->baseSprite = SPRITE_START_ENTITIES + 31;
            enemy->health = 30;
            initSoldierData(entity, 85);

            break;

        case ENEMY_RED_SOLDIER:
            initSoldierEnemy(enemy);
            enemy->health = 33;
            enemy->baseSprite = SPRITE_START_ENTITIES + 33;
            initSoldierData(entity, 78);

            break;
        
        case ENEMY_GREY_FLY:
            initFlyData(entity);
            enemy->health = 10;
            enemy->baseSprite = SPRITE_START_ENTITIES + 15;
            break;
        case ENEMY_RED_FLY:
            initFlyData(entity);
            enemy->health = 15;
            enemy->baseSprite = SPRITE_START_ENTITIES + 17;
            break;
        case ENEMY_BLUE_FLY:
            initFlyData(entity);
            enemy->health = 17;
            enemy->baseSprite = SPRITE_START_ENTITIES + 19;
            break;


        default:
            gLog(LOG_ERR,"Unknown enemy type %d", enemyType);
            break;
    }

    enemy->animationTimer = enemy->animationFrameDuration;

}


Entity* initEnemyDirectional(int x, int y, int type, bool flipDirection){
    Entity* enemy = initEnemy(x, y, type);
    Enemy* e = enemy->data;
    e->flipDirection = flipDirection;
    return enemy;
}


Entity* initEnemy(int x, int y, int type){
    Enemy* enemy = malloc(sizeof(Enemy));
    
    Entity* out = initEntity(x, y, 16, 16, ENTITY_ENEMY, enemy, &enemyUpdate, &enemyOnCollide, &enemyOnDestroy, &enemyClean);

    initEnemyBasedOnType(enemy, out, type);

    return out;
}



void enemyTryJump(Entity* this){
    Enemy* data = this->data;
    if (data->isTouchingGround){
        data->yVelocity = -3.0f;
    }
}

const int HURT_TIMER_MAX = 10;

void gunnerUpdate(Entity* this){
    EntityManager* m = getEntityManager();
    ExtraGunnerData* gunnerData = getExtraEntityData(m, this->extraIndex);
    Enemy* data = this->data;
    Gameplay* gameplay = getGameplay();
    //gLog(LOG_INF,"got here \n eIndex [%d] prt [%p]", this->extraIndex, this);

    {// shooting
        if (gunnerData->cooldown > 0){
            gunnerData->cooldown -= 1; 
        }else if (gameplay->playerX < this->x != data->flipDirection){
            
            // fire
            gunnerData->cooldown = gunnerData->fireRate;
            addEntity(m, initBullet(this->x, this->y, boolToSign(data->flipDirection) * 2.5f, ENTITY_ENEMY));
            
        }
    }

    enemyUpdate(this);
}

void genericEnemyUpdate(Entity* this){
    Enemy* data = this->data;
    {// update values
        this->x += data->xVelocity;
        this->y += data->yVelocity;
    }

    {// animation
        data->animationTimer--;
        if (data->animationTimer <= 0){
            data->animationTimer = data->animationFrameDuration;
            data->animationFrame = ++data->animationFrame % 2;
        }
    }
    
    
    {// draw
        // hurt timer
        data->hurtTimer -= data->hurtTimer > 0;
        
        // color and scale
        float hurtPercentage = (float) data->hurtTimer / HURT_TIMER_MAX;
        float scaleMultiplier = (hurtPercentage * 0.3f) + 1.0f;
        float healthColor =  (1 - (hurtPercentage * 0.4));
        Color c = {255, 255 * healthColor, 255 * healthColor, 255};

        float sizingOffsetX = (1.0f - scaleMultiplier) * 0.5f * this->w;
        float sizingOffsetY = (1.0f - scaleMultiplier) * this->h;


        // draw
        drawFSC(data->baseSprite + data->animationFrame, this->x + sizingOffsetX, this->y + sizingOffsetY, data->flipDirection, scaleMultiplier, c, LAYER_OBJECTS);
    }
}

void enemyUpdate(Entity* this){
    Enemy* data = this->data;
    Gameplay* gameplay = getGameplay();

    {// walking
        data->xVelocity = data->moveSpeed * boolToSign(data->flipDirection);

        // falling
        data->isTouchingGround = collidesWithLevel(gameplay->level, this->x, this->y + this->h + data->yVelocity, this->w, 1);

        if (!data->isTouchingGround){
            data->yVelocity += data->gravity;
        }else {
            data->yVelocity = 0.0f;
        }


        // wall collisions
        if (collidesWithLevel(gameplay->level, this->x + data->xVelocity, this->y, this->w, this->h - 4)){
            data->flipDirection = !data->flipDirection;
        }

        // ceiling collision
        if (data->isTouchingGround == false && data->yVelocity < 0.0f && collidesWithLevel(gameplay->level, this->x, this->y - 2, this->w, 4)){
            data->yVelocity = 0.0f;
        }
    }

    {// marker collisions
        EntityMarker* collidingMarker = getCollidingMarker(gameplay, this);

        if (collidingMarker != 0){
            switch (collidingMarker->id){
                case 4: // jump marker
                    enemyTryJump(this);
                    break;
                case 5: // split jump marker
                    enemyTryJump(this); // TODO
                    break;
                case 6: // decide jump marker
                    if (gameplay->playerY + 8 < this->y){
                        enemyTryJump(this);
                    }
                    break;
                case 7: // turn left marker
                    data->flipDirection = false;
                    break;
                case 8: // turn right marker
                    data->flipDirection = true;
                    break;
            }
        }
    }
    genericEnemyUpdate(this);
}


void flyUpdate(Entity* this){
    Enemy* data = this->data;
    ExtraFlyData* extraData = getExtraEntityData(getEntityManager(), this->extraIndex);

    // move verticaly
    if (extraData->movingDown){
        data->yVelocity += 0.1f;
    }else {
        data->yVelocity -= 0.1f;
    }

    genericEnemyUpdate(this);
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
        
        const char* soundName;
        Enemy* data = this->data;
        switch (data->bodyType) {
            case BODY_FLESH:    soundName = "hurt_small.wav"; break;
            case BODY_ROBOT:    soundName = "hurt_metal.wav"; break;
            case BODY_LARGE:    soundName = "hurt_large.wav"; break;
        }

        playSound(soundName);

    }
}


void spawnGore(Entity* this, EntityManager* e, bool isRobot){
    
    int start = 19 - (isRobot * 3);
    int end = 21 - (isRobot * 3);
    
    Entity* p = initStaticParticle(this->x, this->y, SPRITE_START_EFFECTS + start, 120);
    makeParticleAnimated(p, SPRITE_START_EFFECTS + end, 5);
    makeParticleMove(p, getRandomFloatRange(-1.0f, 1.0f), getRandomFloatRange(-3.0f, 1.0f), 0.1f);
    addEntity(e, p);
}

void spawnRobotHusk(Entity* this, EntityManager* e){
    Entity* p = initStaticParticle(this->x, this->y, SPRITE_START_EFFECTS + 15, 420);
    makeParticleChangeTransparency(p, 255, 0);
    addEntity(e, p);

}

int scoreBasedOnEnemyType(int enemyType){
    switch (enemyType) {
        case ENEMY_GREEN_LIZARD:
        case ENEMY_GREY_LIZARD:
        case ENEMY_PINK_LIZARD:
            return 100;
        case ENEMY_GREEN_SOLDIER:
        case ENEMY_BLUE_SOLDIER:
        case ENEMY_RED_SOLDIER:
            return 200;
        case ENEMY_GREY_SOLDIER:
        case ENEMY_GREY_ROBOT:
        case ENEMY_GREEN_ROBOT:
            return 500;
        
        default:
            return 500;
    }
}

void enemyOnDestroy(Entity* this){
    // spawn gore
    EntityManager* e = getEntityManager();
    Enemy* data = this->data;
    int max = getRandomIntR(3, 5);

    if (data->bodyType == BODY_LARGE){
        max += 3;
    }

    
    for (int i = 0; i < max; i++){
        spawnGore(this, e, data->bodyType == BODY_ROBOT);
    } 

    if (data->bodyType == BODY_ROBOT){
        spawnRobotHusk(this, e);
    } 
    
    

    // spawn explosion
    playSound("small_explosion.wav");
    
    int deathEffectId = 0;
    switch (data->bodyType) {
        case BODY_FLESH: deathEffectId = GRAPHICS_DEATH_SMALL; screenShake(1.2f); break;
        case BODY_ROBOT: deathEffectId = GRAPHICS_DEATH_ROBOT; screenShake(1.8f); break;
        case BODY_LARGE: deathEffectId = GRAPHICS_DEATH_LARGE; screenShake(2.5f); break;
    }
    
    int score = scoreBasedOnEnemyType(data->enemyType);
    addScore(this->x, this->y, score);
    Entity* deathEffect = initExtraGraphic(this->x, this->y, deathEffectId);
    addEntity(e, deathEffect);

    
    
}
void enemyClean(Entity* this){
    if (this->extraIndex != -1){
        removeExtraEntityData(getEntityManager(), this->extraIndex);
    }
}
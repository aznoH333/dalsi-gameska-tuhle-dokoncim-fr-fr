#include "enemy.h"
#include "entities.h"
#include "extraEntityData.h"
#include "gameplay.h"
#include "gframework.h"
#include "bullet.h"
#include "gutil.h"
#include "level.h"
#include "particleEffect.h"
#include "spritedata.h"
#include "extraGraphics.h"
#include "gameprogress.h"
#include <stdlib.h>




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
    ((Enemy*)this->data)->flipDirection = getGameplay()->playerX > this->x;
    
    f->targetHeight = getGameplay()->playerY + getRandomIntR(-20, 20);
    f->movingDown = getGameplay()->playerY > this->y;
    this->updateFunction = &flyUpdate;
    this->extraIndex = allocateExtraEntityData(getEntityManager(), f);
}

void largeFlyUpdate(Entity* this);
void initLargeFly(Entity* this){
    this->h = 20;
    ((Enemy*)this->data)->bodyType = BODY_LARGE;
    ExtraLargeFlyData* e = malloc(sizeof(ExtraLargeFlyData));
    e->movementTimer = 0;
    e->attackTimer = 50;
    this->updateFunction = &largeFlyUpdate;
    this->extraIndex = allocateExtraEntityData(getEntityManager(), e);
    ((Enemy*)this->data)->animationFrameDuration = 3;

}

#define SQUID_FIRE_RATE 90
void squidUpdate(Entity* this);
void initSquid(Entity* this){
    ((Enemy*)this->data)->bodyType = BODY_LARGE;
    ExtraSquidData* e = malloc(sizeof(ExtraSquidData));
    e->jumpCounter = 0;
    e->attackTimer = SQUID_FIRE_RATE;
    this->updateFunction = &squidUpdate;
    this->extraIndex = allocateExtraEntityData(getEntityManager(), e);
    ((Enemy*)this->data)->animationFrameDuration = 7;
}


void lizardBossUpdate(Entity* this);
void initLizardBoss(Entity* this){
    ((Enemy*)this->data)->bodyType = BODY_LARGE;
    ExtraBossData* b = malloc(sizeof(ExtraBossData));
    ((Enemy*)this->data)->animationFrameDuration = 5;
    ((Enemy*)this->data)->animationFrame = 0;
    ((Enemy*)this->data)->animationTimer = 5;

    b->currentPattern = LIZARD_PATTERN_IDLE;
    b->targetX = this->x;
    b->currentPatternTimer = 50;
    b->attackTimer = 0;
    b->anchorXR = this->x;
    b->anchorXL = this->x - 400;


    this->updateFunction = &lizardBossUpdate;
    this->extraIndex = allocateExtraEntityData(getEntityManager(), b);
    ((Enemy*)this->data)->baseSprite = 0;
    this->w = 16;
    this->h = 32;

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
            enemy->animationFrameDuration = 2;
            break;
        case ENEMY_RED_FLY:
            initFlyData(entity);
            enemy->health = 15;
            enemy->baseSprite = SPRITE_START_ENTITIES + 17;
            enemy->animationFrameDuration = 2;
            break;
        case ENEMY_BLUE_FLY:
            initFlyData(entity);
            enemy->health = 17;
            enemy->baseSprite = SPRITE_START_ENTITIES + 19;
            enemy->animationFrameDuration = 2;
            break;

        case ENEMY_LARGE_GREY_FLY:
            initLargeFly(entity);
            enemy->health = 96;
            enemy->baseSprite = SPRITE_START_ENTITIES + 35;
            break;

        case ENEMY_LARGE_RED_FLY:
            initLargeFly(entity);
            enemy->health = 124;
            enemy->baseSprite = SPRITE_START_ENTITIES + 38;
            break;
        
        case ENEMY_SQUID_PINK:
            initSquid(entity);
            enemy->health = 75;
            enemy->baseSprite = SPRITE_START_ENTITIES + 22;
            break;

        case ENEMY_SQUID_BLUE:
            initSquid(entity);
            enemy->health = 110;
            enemy->baseSprite = SPRITE_START_ENTITIES + 25;
            break;

        case ENEMY_LIZARD_BOSS_GREEN:
            initLizardBoss(entity);
            enemy->health = 1000;
            break;
        
        case ENEMY_LIZARD_BOSS_RED:
            initLizardBoss(entity);
            enemy->health = 1500;
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

    {// shooting
        if (gunnerData->cooldown > 0){
            gunnerData->cooldown -= 1; 
        }else if (gameplay->playerX < this->x != data->flipDirection){
            
            // fire
            gunnerData->cooldown = gunnerData->fireRate;
            addEntity(m, initBullet(this->x, this->y, boolToSign(data->flipDirection) * 2.5f, 0.0f, SPRITE_START_EFFECTS + 4, ENTITY_ENEMY, BULLET_FLAG_SPAWN_DECAL));
            
        }
    }

    enemyUpdate(this);
}

void drawEnemySpriteWithOffset(int enemySprite, int spriteOffsetX, int spriteOffsetY, Entity* this){
    Enemy* data = this->data;
    
    // color and scale
    float hurtPercentage = (float) data->hurtTimer / HURT_TIMER_MAX;
    float scaleMultiplier = (hurtPercentage * 0.3f) + 1.0f;
    float healthColor =  (1 - (hurtPercentage * 0.4));
    Color c = {255, 255 * healthColor, 255 * healthColor, 255};

    if (data->flipDirection){
        spriteOffsetX *= -1; 
    }
    
    float sizingOffsetX = (1.0f - scaleMultiplier) * ((spriteOffsetX - 8) / -16.0f) * this->w;
    float sizingOffsetY = (1.0f - scaleMultiplier) * ((spriteOffsetY - 8) / -16.0f) * this->h;

    
    // draw
    drawFSC(enemySprite, this->x + sizingOffsetX + spriteOffsetX, this->y + sizingOffsetY + spriteOffsetY, data->flipDirection, scaleMultiplier, c, LAYER_OBJECTS);

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
        
        drawEnemySpriteWithOffset(data->baseSprite + data->animationFrame, 0, 0, this);
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

    data->yVelocity = (extraData->targetHeight - this->y) * 0.02f;

    data->xVelocity += boolToSign(data->flipDirection) * 0.02f;

    genericEnemyUpdate(this);
}
#define LARGE_FLY_PROJ_SPEED 2.3f
void largeFlyUpdate(Entity* this){
    Enemy* data = this->data;
    ExtraLargeFlyData* extraData = getExtraEntityData(getEntityManager(), this->extraIndex);
    drawEnemySpriteWithOffset(data->baseSprite + 2, 0, 16, this);
    genericEnemyUpdate(this);

    float playerX = getGameplay()->playerX;
    float playerY = getGameplay()->playerY;

    // move
    this->x += sin(extraData->movementTimer * 0.03f) * 0.3f;
    this->y += cos(extraData->movementTimer * 0.05f) * 0.6f;


    // attack
    if (extraData->attackTimer < 15 && extraData->attackTimer % 5 == 0){
        // calc direction
        float dir = dirTowards(playerX, playerY, this->x, this->y);
        
        addEntity(getEntityManager(), initBullet(this->x, this->y, sin(dir) * LARGE_FLY_PROJ_SPEED, cos(dir) * LARGE_FLY_PROJ_SPEED, SPRITE_START_EFFECTS + 6 - (extraData->attackTimer == 0), ENTITY_ENEMY_PROJECTILE, 0));
    }

    if (extraData->attackTimer == 0){
        extraData->attackTimer = 200;
    }


    // update values
    extraData->movementTimer++;
    extraData->attackTimer -= extraData->attackTimer > 0;
    data->flipDirection = playerX > this->x;
}


void squidUpdate(Entity* this){
    Enemy* data = this->data;
    ExtraSquidData* extraData = getExtraEntityData(getEntityManager(), this->extraIndex);
    genericEnemyUpdate(this);
    drawEnemySpriteWithOffset(data->baseSprite - 1, 0, -16, this);

    bool isOnGround = collidesWithLevel(getGameplay()->level, this->x, this->y + 16, 16, 1);

    if (isOnGround){
        data->yVelocity = -1.0f - (1.5f * (extraData->jumpCounter % 8 == 0));
        extraData->jumpCounter++;
        data->xVelocity = getRandomFloatRange(-1.0f, 1.0f);

        data->flipDirection = this->x < getGameplay()->playerX;
    }


    // shooting
    extraData->attackTimer--;
    if (extraData->attackTimer == 0){
        addEntity(getEntityManager(), initBullet(this->x, this->y, boolToSign(data->flipDirection) * 2.5f, 0.0f, SPRITE_START_EFFECTS + 1, ENTITY_ENEMY, BULLET_FLAG_ANIMATED | BULLET_FLAG_PHASING));

        extraData->attackTimer = SQUID_FIRE_RATE;
    }

    data->yVelocity += 0.1f;
}

void lizardBossPickNextState(Entity* this, Enemy* data, ExtraBossData* extraData){
    switch (extraData->currentPattern) {
        case LIZARD_PATTERN_IDLE:
            extraData->currentPattern = LIZARD_PATTERN_SHOOT;
            extraData->currentPatternTimer = 140;
            break;
        case LIZARD_PATTERN_SHOOT:
            if (randomChance(0.5f)){
                extraData->currentPattern = LIZARD_PATTERN_JUMP;
            }else {
                extraData->currentPattern = LIZARD_PATTERN_RUN;
            }
            break;
        case LIZARD_PATTERN_RUN:
        case LIZARD_PATTERN_JUMP:
            extraData->currentPattern = LIZARD_PATTERN_IDLE;
            extraData->currentPatternTimer = 50;
            break;
    }
}

void lizardBossIdle(Entity* this, Enemy* data, ExtraBossData* extraData){
    data->animationFrameDuration = 4;
    data->flipDirection = this->x < getGameplay()->playerX;
    if (extraData->currentPatternTimer <= 0){
        lizardBossPickNextState(this, data, extraData);
    }
}

void lizardBossShoot(Entity* this, Enemy* data, ExtraBossData* extraData){
    data->animationFrameDuration = 12;
    extraData->attackTimer--;
    data->flipDirection = this->x < getGameplay()->playerX;


    if (extraData->attackTimer <= 0){
        data->animationFrame = (data->animationFrame + 1) % 2;
        extraData->attackTimer = 40 + (10 * (data->enemyType == ENEMY_LIZARD_BOSS_GREEN));
        float bulletSpeed = boolToSign(data->flipDirection) * (2.0f + (1.2f * ( data->enemyType == ENEMY_LIZARD_BOSS_RED )));
        addEntity(getEntityManager(), initBullet(this->x, this->y, bulletSpeed, 0.0f, SPRITE_START_EFFECTS + 2, ENTITY_ENEMY, BULLET_FLAG_PHASING | BULLET_FLAG_ANIMATED | BULLET_FLAG_SPAWN_DECAL));

    }
}

void lizardBossUpdate(Entity* this){
    Enemy* data = this->data;
    ExtraBossData* extraData = getExtraEntityData(getEntityManager(), this->extraIndex);

    // simplified generic update
    data->hurtTimer -= data->hurtTimer > 0;
    this->x += data->xVelocity;
    this->y += data->yVelocity;

    bool isOnGround = collidesWithLevel(getGameplay()->level, this->x, this->y + 16, 16, 1);

    int variantSpriteOffset = (data->enemyType == ENEMY_LIZARD_BOSS_RED) * 7;
    // legs
    drawEnemySpriteWithOffset(SPRITE_START_ENTITIES + 43 + (data->animationFrame * 2) + variantSpriteOffset, -8, 0, this);
    drawEnemySpriteWithOffset(SPRITE_START_ENTITIES + 44 + (data->animationFrame * 2) + variantSpriteOffset, 8, 0, this);

    // head
    drawEnemySpriteWithOffset(SPRITE_START_ENTITIES + 41 + data->animationFrame + variantSpriteOffset, -1, -11, this);
    
    // on ground update
    if (isOnGround){
        extraData->currentPatternTimer--;
        data->animationTimer--;

        if (data->animationTimer == 0){
            data->animationTimer = data->animationFrameDuration;
            data->animationFrame = (data->animationFrame + 1) % 2;
        }

    }
    
    // pattern update
    switch(extraData->currentPattern){
        case LIZARD_PATTERN_IDLE:
            lizardBossIdle(this, data, extraData);
            break;
        case LIZARD_PATTERN_SHOOT:
            lizardBossShoot(this, data, extraData);
            break;
        case LIZARD_PATTERN_RUN:
            break;
        case LIZARD_PATTERN_JUMP:
            break;
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
        case ENEMY_GREY_FLY:
        case ENEMY_RED_FLY:
        case ENEMY_BLUE_FLY:
            return 100;
        case ENEMY_GREEN_SOLDIER:
        case ENEMY_BLUE_SOLDIER:
        case ENEMY_RED_SOLDIER:
            return 200;
        case ENEMY_GREY_SOLDIER:
        case ENEMY_GREY_ROBOT:
        case ENEMY_GREEN_ROBOT:
            return 500;
        case ENEMY_LARGE_GREY_FLY:
        case ENEMY_LARGE_RED_FLY:
            return 1000;    
        case ENEMY_SQUID_PINK:
        case ENEMY_SQUID_BLUE:
            return 2000;
        case ENEMY_LIZARD_BOSS_GREEN:
        case ENEMY_LIZARD_BOSS_RED:
            return 5000;
        
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
        if (data->isTouchingGround){
            spawnRobotHusk(this, e);
        }else {
            for (int i = 0; i < max; i++){
                spawnGore(this, e, data->bodyType == BODY_ROBOT);
            } 
        }
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
#ifndef EXTRA_ENTITY_DATA
#define EXTRA_ENTITY_DATA

struct ExtraGunnerData{
    int cooldown;
    int fireRate;
};
typedef struct ExtraGunnerData ExtraGunnerData;

struct ExtraFlyData{
    int targetHeight;
    char movingDown;
};
typedef struct ExtraFlyData ExtraFlyData;


struct ExtraLargeFlyData{
    int movementTimer;
    int attackTimer;
};
typedef struct ExtraLargeFlyData ExtraLargeFlyData;


struct ExtraSquidData{
    int jumpCounter;
    int attackTimer;
};
typedef struct ExtraSquidData ExtraSquidData;

#define LIZARD_PATTERN_IDLE 0
#define LIZARD_PATTERN_SHOOT 1
#define LIZARD_PATTERN_RUN 2
#define LIZARD_PATTERN_JUMP 3
struct ExtraBossData{
    int targetX;
    int anchorXR;
    int anchorXL;
    int currentPattern;
    int currentPatternTimer;
    int attackTimer;
    char wanderTimer;
    char wanderDirection;
};
typedef struct ExtraBossData ExtraBossData;

#endif
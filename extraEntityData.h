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



#endif
#ifndef LEVEL 
#define LEVEL 


#include "gfiles.h"
#include "gcollections.h"



#define LEVEL_DRAW_GAME 0
#define LEVEL_DRAW_EDITOR 1
#define LEVEL_DRAW_EDITOR_TILES 2
#define LEVEL_DRAW_EDITOR_BACKGROUND 3
#define LEVEL_DRAW_CAMERA_VIEW 4
#define LEVEL_DRAW_COUNT 5

#define LEVEL_NAME_LENGTH 10

struct Level {
    char** tiles;
    char** background;
    int width;
    int height;
    char waterType;
    char songId;
    File* levelFile;
    Vector* entityeMarkers;
    char name[LEVEL_NAME_LENGTH];
    
};
typedef struct Level Level;


Level* loadLevel(const char* levePath);                         // loads level into memory returns pointer to level
void saveLevel(Level* lvl);                                     // saves level to file specified when loading level
void unloadLevel(Level* level);                                 // unloads level and frees asociated pointers
void resizeLevel(Level* level, int newWidth, int newHeight);    // resizes level
void drawLevel(Level* lvl, int drawLevel);                      // draws level and background
bool collidesWithLevel(Level* lvl, int x, int y, int w, int h);



#endif
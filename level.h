#ifndef LEVEL 
#define LEVEL 


#include "gfiles.h"
#include "gvector.h"





struct Level {
    char** tiles;
    char** background;
    int width;
    int height;
    File* levelFile;
    Vector entityeMarkers;

};
typedef struct Level Level;


Level* loadLevel(const char* levePath);                         // loads level into memory returns pointer to level
void saveLevel(Level* lvl);                                     // saves level to file specified when loading level
void unloadLevel(Level* level);                                 // unloads level and frees asociated pointers
void resizeLevel(Level* level, int newWidth, int newHeight);    // resizes level
void drawLevel(Level* lvl);                                     // draws level and background
void drawEntityMarkers(Level* lvl);                             // draws entity markers (should only by used by level editor)


#endif
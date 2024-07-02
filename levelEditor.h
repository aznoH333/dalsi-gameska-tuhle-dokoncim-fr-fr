#ifndef LEVEL_EDITOR
#define LEVEL_EDITOR

#include "level.h"
#include "entities.h"
#include "gframework.h"
#include "spritedata.h"


struct LevelEditor{
    Level* level;
    
    // operations
    int currentOperation;

    // cursor
    int cursorX;
    int cursorY;
    int cursorInWorldX;
    int cursorInWorldY;
    
    // camera
    Vector2 cameraPos;
    float cameraZoom;

    // resizing
    int newWidth;
    int newHeight;

    // tile selection
    int selectedTile;
    int placeMode;

    int viewMode;
    
};
typedef struct LevelEditor LevelEditor;

LevelEditor* getLevelEditor();
LevelEditor* initLevelEditor();
void unloadLevelEditor(LevelEditor* editor);
void updateLevelEditor(LevelEditor* editor);

#endif
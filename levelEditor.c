#ifndef LEVEL_EDITOR
#define LEVEL_EDITOR

#include "level.c"
#include "gframework.c"

#define OPERATION_EDIT 0
#define OPERATION_RESIZE 1
#define OPERATION_SELECT_TILE 2

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
};
typedef struct LevelEditor LevelEditor;



LevelEditor* initLevelEditor(const char* levelPath){
    LevelEditor* out = malloc(sizeof(LevelEditor));

    out->level = loadLevel(levelPath);

    out->cameraPos = (Vector2) {0, 0};
    out->cameraZoom = 1.0f;

    out->currentOperation = OPERATION_EDIT;
    out->newHeight = 0;
    out->newHeight = 0;

    out->selectedTile = 1;
    return out;
}



void unloadLevelEditor(LevelEditor* editor){
    unloadLevel(editor->level);
    free(editor);
}


#define CURSOR_SPRITE 0
#define OUT_OF_BOUNDS_SPRITE 1
#define NEW_BOUNDS_SPRITE 2

#define TILE_SPRITE_OFFSET 8
#define MAX_TILES 2
#define TILE_PICKER_X 0
#define TILE_PICKER_Y 0
#define TILE_PICKER_WIDTH 8

void updateLevelEditor(LevelEditor* editor){
    drawLevel(editor->level);

    
    // draw level bounds
    {
        for (int i = -1; i < editor->level->width + 1; i++){
            draw(OUT_OF_BOUNDS_SPRITE, i * 32, -32, LAYER_UI);
            draw(OUT_OF_BOUNDS_SPRITE, i * 32, (editor->level->height) * 32, LAYER_UI);
        }

        for (int i = 0; i < editor->level->height; i++){
            draw(OUT_OF_BOUNDS_SPRITE, -32, i * 32, LAYER_UI);
            draw(OUT_OF_BOUNDS_SPRITE, (editor->level->width) * 32, i * 32, LAYER_UI);
        }

        if (editor->currentOperation == OPERATION_RESIZE){
            for (int i = -1; i < editor->newWidth + 1; i++){
                draw(NEW_BOUNDS_SPRITE, i * 32, -32, LAYER_UI);
                draw(NEW_BOUNDS_SPRITE, i * 32, (editor->newHeight) * 32, LAYER_UI);
            }

            for (int i = 0; i < editor->newHeight; i++){
                draw(NEW_BOUNDS_SPRITE, -32, i * 32, LAYER_UI);
                draw(NEW_BOUNDS_SPRITE, (editor->newWidth) * 32, i * 32, LAYER_UI);
            }
        }
    }
    // level sizing
    {
        // start resizing
        if (IsKeyPressed(KEY_SPACE) && editor->currentOperation == OPERATION_EDIT){
            editor->newHeight = editor->level->height;
            editor->newWidth = editor->level->width;
            editor->currentOperation = OPERATION_RESIZE;
        }else if (IsKeyPressed(KEY_SPACE) && editor->currentOperation == OPERATION_RESIZE){
            editor->currentOperation = OPERATION_EDIT;
            resizeLevel(editor->level, editor->newWidth, editor->newHeight);
        }

        if (editor->currentOperation == OPERATION_RESIZE){
            if (IsKeyPressed(KEY_LEFT) || IsKeyPressedRepeat(KEY_LEFT)){
                editor->newWidth -= 1;
            }else if (IsKeyPressed(KEY_RIGHT) || IsKeyPressedRepeat(KEY_RIGHT)){
                editor->newWidth += 1;
            }

            if (IsKeyPressed(KEY_UP) ||IsKeyPressedRepeat(KEY_UP)){
                editor->newHeight -= 1;
            }else if (IsKeyPressed(KEY_DOWN) || IsKeyPressedRepeat(KEY_DOWN)){
                editor->newHeight += 1;
            }
        }
    }
    
    
    // cusror
    if (editor->currentOperation == OPERATION_EDIT){
        Vector2 mousePos = getInWorldMousePosition();

        editor->cursorInWorldX = mousePos.x / 32;
        editor->cursorInWorldY = mousePos.y / 32;

        editor->cursorX = editor->cursorInWorldX * 32;
        editor->cursorY = editor->cursorInWorldY * 32;

        draw(CURSOR_SPRITE, editor->cursorX, editor->cursorY, LAYER_UI);

        // place tiles
        if (
            checkBoxCollisions(editor->cursorInWorldX, editor->cursorInWorldY, 1, 1, 0, 0, editor->level->width, editor->level->height)
        ){
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)){
                editor->level->tiles[editor->cursorInWorldX][editor->cursorInWorldY] = editor->selectedTile;
            }else if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)){
                editor->level->tiles[editor->cursorInWorldX][editor->cursorInWorldY] = 0;
            }
        }
    }


    // tile selection
    {
        if (IsKeyPressed(KEY_E) && editor->currentOperation == OPERATION_EDIT){
            editor->currentOperation = OPERATION_SELECT_TILE;
        }else if (IsKeyPressed(KEY_E) && editor->currentOperation == OPERATION_SELECT_TILE){
            editor->currentOperation = OPERATION_EDIT;
        }


        if (editor->currentOperation == OPERATION_SELECT_TILE){
            // draw tiles
            for (int tileId = 0; tileId < MAX_TILES; tileId++){
                int tileX = (tileId % TILE_PICKER_WIDTH) * 64;
                int tileY = (tileId / TILE_PICKER_WIDTH) * 64;

                drawS(TILE_SPRITE_OFFSET + tileId, tileX, tileY, 2.0f, LAYER_STATIC_UI);
            }
        }


        // cursor
        Vector2 mousePos = getOnScreenMousePosition();


        if (mousePos.x < TILE_PICKER_WIDTH * 64){
            editor->cursorX = mousePos.x / 64;
            editor->cursorY = mousePos.y / 64;
            int newTileId = editor->cursorX + (editor->cursorY * TILE_PICKER_WIDTH);

            if (newTileId < MAX_TILES){
                drawS(CURSOR_SPRITE, editor->cursorX * 64, editor->cursorY * 64, 2.0f, LAYER_STATIC_UI);

                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                    editor->selectedTile = newTileId + 1;
                }
            }
        }
        
    }

    // camera movement
    {
        float cameraSpeed = 2 * (editor->cameraZoom);

        if (IsKeyDown(KEY_LEFT_SHIFT)){
            cameraSpeed *= 4;
        }
        
        if (IsKeyDown(KEY_W)){
            editor->cameraPos.y -= cameraSpeed;
        }else if (IsKeyDown(KEY_S)){
            editor->cameraPos.y += cameraSpeed;
        }

        if (IsKeyDown(KEY_A)){
            editor->cameraPos.x -= cameraSpeed;
        }else if (IsKeyDown(KEY_D)){
            editor->cameraPos.x += cameraSpeed;
        }


        if (IsKeyDown(KEY_C)){
            resetCameraZoom();
        }

        addCameraZoom(GetMouseWheelMove() * 0.25f);
        editor->cameraZoom -= GetMouseWheelMove() * 0.25f;

        setCameraPos(editor->cameraPos);
    }

    // file saving
    {
        if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_S)){
            saveLevel(editor->level);
        }
    }
}


#endif
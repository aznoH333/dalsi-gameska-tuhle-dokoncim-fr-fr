#ifndef LEVEL_EDITOR
#define LEVEL_EDITOR

#include "level.c"
#include "gframework.c"

struct LevelEditor{
    Level* level;
    
    // cursor
    int cursorX;
    int cursorY;
    int cursorInWorldX;
    int cursorInWorldY;
    
    // camera
    Vector2 cameraPos;

    // resizing
    bool isResizing;
    int newWidth;
    int newHeight;
    int resizeCooldown;

    // tile selection
    int selectedTile;
};
typedef struct LevelEditor LevelEditor;



LevelEditor* initLevelEditor(const char* levelPath){
    LevelEditor* out = malloc(sizeof(LevelEditor));

    out->level = loadLevel(levelPath);

    out->cameraPos = (Vector2) {0, 0};

    out->isResizing = false;
    out->newHeight = 0;
    out->newHeight = 0;
    out->resizeCooldown = 0;

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
#define DEFAULT_RESIZE_COOLDOWN 15
void updateLevelEditor(LevelEditor* editor){
    drawLevel(editor->level);

    
    // draw level bounds
    {
        for (int i = -1; i < editor->level->width + 1; i++){
            draw(OUT_OF_BOUNDS_SPRITE, i * 32, -32);
            draw(OUT_OF_BOUNDS_SPRITE, i * 32, (editor->level->height) * 32);
        }

        for (int i = 0; i < editor->level->height; i++){
            draw(OUT_OF_BOUNDS_SPRITE, -32, i * 32);
            draw(OUT_OF_BOUNDS_SPRITE, (editor->level->width) * 32, i * 32);
        }

        if (editor->isResizing){
            for (int i = -1; i < editor->newWidth + 1; i++){
                draw(NEW_BOUNDS_SPRITE, i * 32, -32);
                draw(NEW_BOUNDS_SPRITE, i * 32, (editor->newHeight) * 32);
            }

            for (int i = 0; i < editor->newHeight; i++){
                draw(NEW_BOUNDS_SPRITE, -32, i * 32);
                draw(NEW_BOUNDS_SPRITE, (editor->newWidth) * 32, i * 32);
            }
        }
    }
    // level sizing
    {
        // start resizing
        if (IsKeyPressed(KEY_SPACE) && editor->isResizing == false){
            editor->newHeight = editor->level->height;
            editor->newWidth = editor->level->width;
            editor->isResizing = true;
        }else if (IsKeyPressed(KEY_SPACE) && editor->isResizing == true){
            editor->isResizing = false;
            resizeLevel(editor->level, editor->newWidth, editor->newHeight);
        }

        if (editor->isResizing){

            if (editor->resizeCooldown == 0){

                if (IsKeyDown(KEY_LEFT)){
                    editor->newWidth -= 1;
                    editor->resizeCooldown = DEFAULT_RESIZE_COOLDOWN;
                }else if (IsKeyDown(KEY_RIGHT)){
                    editor->newWidth += 1;
                    editor->resizeCooldown = DEFAULT_RESIZE_COOLDOWN;
                }

                if (IsKeyDown(KEY_UP)){
                    editor->newHeight -= 1;
                    editor->resizeCooldown = DEFAULT_RESIZE_COOLDOWN;
                }else if (IsKeyDown(KEY_DOWN)){
                    editor->newHeight += 1;
                    editor->resizeCooldown = DEFAULT_RESIZE_COOLDOWN;
                }

            }else {
                editor->resizeCooldown--;
            }

        }
    }
    
    
    // cusror
    {
        Vector2 mousePos = getInWorldMousePosition();

        editor->cursorInWorldX = mousePos.x / 32;
        editor->cursorInWorldY = mousePos.y / 32;

        editor->cursorX = editor->cursorInWorldX * 32;
        editor->cursorY = editor->cursorInWorldY * 32;

        draw(CURSOR_SPRITE, editor->cursorX, editor->cursorY);

        // place tiles
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && 
            checkBoxCollisions(editor->cursorInWorldX, editor->cursorInWorldY, 1, 1, 0, 0, editor->level->width, editor->level->height)
        ){
            editor->level->tiles[editor->cursorInWorldX][editor->cursorInWorldY] = editor->selectedTile;
        }
    }

    // camera movement
    {
        float cameraSpeed = 2;

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

        setCameraPos(editor->cameraPos);
    }
}


#endif
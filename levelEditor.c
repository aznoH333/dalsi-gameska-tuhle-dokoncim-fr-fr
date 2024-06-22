#include "levelEditor.h"



#define OPERATION_EDIT 0
#define OPERATION_RESIZE 1
#define OPERATION_SELECT_TILE 2
#define OPERATION_SELECT_ENTITY 3


#define PLACE_MODE_TILES 0
#define PLACE_MODE_ENTITES 1
#define PLACE_MODE_BACKGROUND 2





LevelEditor* initLevelEditor(const char* levelPath){
    LevelEditor* out = malloc(sizeof(LevelEditor));

    out->level = loadLevel(levelPath);

    out->cameraPos = (Vector2) {0, 0};
    out->cameraZoom = 1.0f;

    out->currentOperation = OPERATION_EDIT;
    out->newHeight = 0;
    out->newHeight = 0;

    out->selectedTile = 1;
    out->placeMode = PLACE_MODE_TILES;
    return out;
}



void unloadLevelEditor(LevelEditor* editor){
    unloadLevel(editor->level);
    free(editor);
}

#define TILE_PICKER_X 0
#define TILE_PICKER_Y 0
#define TILE_PICKER_WIDTH 8


void tileSelector(LevelEditor* editor, int operationType);
void updateLevelEditor(LevelEditor* editor){
    drawLevel(editor->level);
    drawEntityMarkers(editor->level);
    
    // draw level bounds
    {
        for (int i = -1; i < editor->level->width + 1; i++){
            draw(OUT_OF_BOUNDS_SPRITE, i * 16, -16, LAYER_UI);
            draw(OUT_OF_BOUNDS_SPRITE, i * 16, (editor->level->height) * 16, LAYER_UI);
        }

        for (int i = 0; i < editor->level->height; i++){
            draw(OUT_OF_BOUNDS_SPRITE, -16, i * 16, LAYER_UI);
            draw(OUT_OF_BOUNDS_SPRITE, (editor->level->width) * 16, i * 16, LAYER_UI);
        }

        if (editor->currentOperation == OPERATION_RESIZE){
            for (int i = -1; i < editor->newWidth + 1; i++){
                draw(NEW_BOUNDS_SPRITE, i * 16, -16, LAYER_UI);
                draw(NEW_BOUNDS_SPRITE, i * 16, (editor->newHeight) * 16, LAYER_UI);
            }

            for (int i = 0; i < editor->newHeight; i++){
                draw(NEW_BOUNDS_SPRITE, -16, i * 16, LAYER_UI);
                draw(NEW_BOUNDS_SPRITE, (editor->newWidth) * 16, i * 16, LAYER_UI);
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

        editor->cursorInWorldX = mousePos.x / 16;
        editor->cursorInWorldY = mousePos.y / 16;

        editor->cursorX = editor->cursorInWorldX * 16;
        editor->cursorY = editor->cursorInWorldY * 16;

        draw(TILE_CURSOR, editor->cursorX, editor->cursorY, LAYER_UI);

        // place tiles
        if (
            checkBoxCollisions(editor->cursorInWorldX, editor->cursorInWorldY, 1, 1, 0, 0, editor->level->width, editor->level->height)
        ){
            if (editor->placeMode == PLACE_MODE_ENTITES){ // entity markers
                if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                    vectorPush(&editor->level->entityeMarkers, initEntityMarkerBasic(editor->selectedTile, editor->cursorInWorldX, editor->cursorInWorldY));
                }else if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)){
                    // find and remove entities
                    for (int i = 0; i < editor->level->entityeMarkers.elementCount; i++){
                        EntityMarker* m = vectorGet(&editor->level->entityeMarkers, i);

                        if (m->x == editor->cursorInWorldX && m->y == editor->cursorInWorldY){
                            vectorRemove(&editor->level->entityeMarkers, i);
                            i--;
                        }
                    }
                }
            }else { // tiles
                char** targetArray = (editor->placeMode == PLACE_MODE_TILES ? editor->level->tiles : editor->level->background);
                
                if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)){
                    targetArray[editor->cursorInWorldX][editor->cursorInWorldY] = editor->selectedTile;
                }else if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)){
                    targetArray[editor->cursorInWorldX][editor->cursorInWorldY] = 0;
                }
            }
        }
    }


    // tile selection
    tileSelector(editor, OPERATION_SELECT_TILE);

    // entity selection
    tileSelector(editor, OPERATION_SELECT_ENTITY);

    
    {
        // background controls
        if(IsKeyPressed(KEY_B) && editor->placeMode == PLACE_MODE_TILES){
            editor->placeMode = PLACE_MODE_BACKGROUND;
        }else if (IsKeyPressed(KEY_B) && editor->placeMode == PLACE_MODE_BACKGROUND){
            editor->placeMode = PLACE_MODE_TILES;
        }
    }

    // camera movement
    {
        float cameraSpeed = max(2 * (editor->cameraZoom), 1);

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

    // displaying state
    {
        // mode display
        {
            const char* currentMode;

            switch (editor->currentOperation) {
                case OPERATION_EDIT: currentMode = "edit mode"; break;
                case OPERATION_RESIZE: currentMode = "resize level"; break;
                case OPERATION_SELECT_TILE: currentMode = "select tile"; break;
                case OPERATION_SELECT_ENTITY: currentMode = "select entity"; break;
            }
            
            textF(currentMode, 540, 8);
        }

        // place mode display
        if (editor->currentOperation == OPERATION_EDIT)
        {
            {   // display type
                const char* currentMode;

                switch (editor->placeMode) {
                    case PLACE_MODE_ENTITES: currentMode = "ent"; break;
                    case PLACE_MODE_TILES: currentMode = "tile"; break;
                    case PLACE_MODE_BACKGROUND: currentMode = "back"; break;
                }
                
                textF("placing %s", 1050, 8, currentMode);
            }

            // show tile preview
            {   
                int spriteIndex = editor->selectedTile;
                switch (editor->placeMode) {
                    case PLACE_MODE_TILES:
                    case PLACE_MODE_BACKGROUND:
                        spriteIndex += SPRITE_START_TILES - 1;
                        break;
                    case PLACE_MODE_ENTITES:
                        spriteIndex += SPRITE_START_MARKERS;
                        break;
                }
                
                drawS(spriteIndex, 1140, 32, 6.0, LAYER_STATIC_UI);
            }

            // show coordinates
            if (checkBoxCollisions(0, 0, editor->level->width, editor->level->height, editor->cursorInWorldX, editor->cursorInWorldY, 1, 1)){
                textF("x %d y %d", 1110, 136, editor->cursorInWorldX, editor->cursorInWorldY);
            }
            
        }

        
    }
}

void tileSelector(LevelEditor* editor, int operationType){
    int key;
    int itemCount;
    int itemStart;
    int placeMode;
    switch (operationType) {
        case OPERATION_SELECT_ENTITY:   key = KEY_Q; itemCount = SPRITE_COUNT_MARKERS;  itemStart = SPRITE_START_MARKERS;   placeMode = PLACE_MODE_ENTITES; break;
        case OPERATION_SELECT_TILE:     key = KEY_E; itemCount = SPRITE_COUNT_TILES;    itemStart = SPRITE_START_TILES;     placeMode = (editor->placeMode == PLACE_MODE_BACKGROUND ? PLACE_MODE_BACKGROUND : PLACE_MODE_TILES); break;
    }

    
    
    // key operations
    if (IsKeyPressed(key) && editor->currentOperation == OPERATION_EDIT){
        editor->currentOperation = operationType;
    }else if (IsKeyPressed(key) && editor->currentOperation == operationType){
        editor->currentOperation = OPERATION_EDIT;
    }

    if (editor->currentOperation == operationType){
        // draw items
        for (int entityId = 0; entityId <= itemCount; entityId++){
            int entityX = (entityId % TILE_PICKER_WIDTH) * 32;
            int entityY = (entityId / TILE_PICKER_WIDTH) * 32;

            drawS(itemStart + entityId, entityX, entityY, 2.0f, LAYER_STATIC_UI);
        }


        // cursor
        Vector2 mousePos = getOnScreenMousePosition();


        if (mousePos.x < TILE_PICKER_WIDTH * 32){
            editor->cursorX = mousePos.x / 32;
            editor->cursorY = mousePos.y / 32;
            int newEntityId = editor->cursorX + (editor->cursorY * TILE_PICKER_WIDTH);

            if (newEntityId <= itemCount){
                drawS(TILE_CURSOR, editor->cursorX * 32, editor->cursorY * 32, 2.0f, LAYER_STATIC_UI);

                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                    editor->selectedTile = newEntityId + (operationType == OPERATION_SELECT_TILE); // hacky workaround
                    editor->placeMode = placeMode;
                }
            }
        }
    }
}
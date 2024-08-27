#include "levelEditor.h"
#include "level.h"



#define OPERATION_EDIT 0
#define OPERATION_RESIZE 1
#define OPERATION_SELECT_TILE 2
#define OPERATION_SELECT_ENTITY 3
#define OPERATION_MENU 4
#define MENU_ITEMS 6
#define PLACE_MODE_TILES 0
#define PLACE_MODE_ENTITES 1
#define PLACE_MODE_BACKGROUND 2

#define PLACE_TOOL_PENCIL 0
#define PLACE_TOOL_BUCKET 1

LevelEditor* initLevelEditor(){
    LevelEditor* out = malloc(sizeof(LevelEditor));

    out->level = 0;

    out->cameraPos = (Vector2) {0, 0};
    out->cameraZoom = 1.0f;

    out->currentOperation = OPERATION_EDIT;
    out->newHeight = 0;
    out->newHeight = 0;

    out->selectedTile = 1;
    out->placeMode = PLACE_MODE_TILES;
    out->currentPlaceTool = PLACE_TOOL_PENCIL;

    out->viewMode = LEVEL_DRAW_EDITOR;

    out->currentSelectedOption = 0;
    return out;
}

LevelEditor* editorInstance = 0;
LevelEditor* getLevelEditor(){
    if (editorInstance == 0) {
        editorInstance = initLevelEditor();
    }
    return editorInstance;
}



void loadLevelEditorLevel(LevelEditor* editor, const char* path){
    editor->level = loadLevel(path);
}


void unloadLevelEditor(LevelEditor* editor){
    unloadLevel(editor->level);
    editor->level = 0;
}

void disposeLevelEditor(LevelEditor* editor){
    if (editor->level != 0)
        unloadLevelEditor(editor);
    free(editor);
}

#define TILE_PICKER_X 0
#define TILE_PICKER_Y 0
#define TILE_PICKER_WIDTH 8

void fillTiles(char** targetArray, int sizeX, int sizeY, int targetX, int targetY, int targetTileId, int placingTile){
    if (targetTileId == placingTile) return;
    
    if (checkBoxCollisions(0, 0, sizeX, sizeY, targetX, targetY, 1, 1) && targetArray[targetX][targetY] == targetTileId){
        targetArray[targetX][targetY] = placingTile;

        fillTiles(targetArray, sizeX, sizeY, targetX - 1, targetY, targetTileId, placingTile);
        fillTiles(targetArray, sizeX, sizeY, targetX + 1, targetY, targetTileId, placingTile);
        fillTiles(targetArray, sizeX, sizeY, targetX, targetY - 1, targetTileId, placingTile);
        fillTiles(targetArray, sizeX, sizeY, targetX, targetY + 1, targetTileId, placingTile);

    }

    return;
}


void useEditTool(LevelEditor* editor){
    char** targetArray = (editor->placeMode == PLACE_MODE_TILES ? editor->level->tiles : editor->level->background);
    
    int placingTile;

    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)){
        placingTile = editor->selectedTile;
    }else if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)){
        placingTile = 0;
    }else {
        return;
    }
    
    
    
    switch (editor->currentPlaceTool) {
        case PLACE_TOOL_PENCIL: targetArray[editor->cursorInWorldX][editor->cursorInWorldY] = placingTile; break;
        case PLACE_TOOL_BUCKET: 
            fillTiles(
                targetArray, 
                editor->level->width, 
                editor->level->height, 
                editor->cursorInWorldX, 
                editor->cursorInWorldY, 
                targetArray[editor->cursorInWorldX][editor->cursorInWorldY], 
                placingTile); 
            break;
    }
    
        
    
}



void tileSelector(LevelEditor* editor, int operationType);
void drawUiIntSelector(char* value, int maxValue, int x, int y, const char* textName, bool isSelected);
void exitMenuButton(LevelEditor* editor);
void saveLevelButton(LevelEditor* editor);
void resetLevelButton(LevelEditor* editor);
void editorMenuButton(LevelEditor* editor, const char* text, int index, int x, int y, void (*useFunc)(LevelEditor* editor));
void updateLevelEditor(LevelEditor* editor){
    drawLevel(editor->level, editor->viewMode);
    
    
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

            // mouse resize
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)){
                Vector2 mousePos = getInWorldMousePosition();
                editor->newWidth = mousePos.x / 16;
                editor->newHeight = mousePos.y / 16;
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
                    vectorPush(editor->level->entityeMarkers, initEntityMarkerBasic(editor->selectedTile, editor->cursorInWorldX, editor->cursorInWorldY));
                }else if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)){
                    // find and remove entities
                    for (int i = 0; i < editor->level->entityeMarkers->elementCount; i++){
                        EntityMarker* m = vectorGet(editor->level->entityeMarkers, i);

                        if (m->x == editor->cursorInWorldX && m->y == editor->cursorInWorldY){
                            vectorRemove(editor->level->entityeMarkers, i);
                            i--;
                        }
                    }
                }
            }else { // tiles
                
                useEditTool(editor);
            }
        }
    }


    // tile selection
    tileSelector(editor, OPERATION_SELECT_TILE);

    // entity selection
    tileSelector(editor, OPERATION_SELECT_ENTITY);

    // background controls
    {
        
        if(IsKeyPressed(KEY_B) && editor->placeMode == PLACE_MODE_TILES){
            editor->placeMode = PLACE_MODE_BACKGROUND;
        }else if (IsKeyPressed(KEY_B) && editor->placeMode == PLACE_MODE_BACKGROUND){
            editor->placeMode = PLACE_MODE_TILES;
        }
    }

    // camera movement
    if (editor->currentOperation != OPERATION_MENU){
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

    // toggle view mode
    {
        if (IsKeyPressed(KEY_V) && editor->currentOperation != OPERATION_MENU){
            editor->viewMode++;
            editor->viewMode %= LEVEL_DRAW_COUNT;
        }
    }

    // tool switching
    {
        if (IsKeyPressed(KEY_F) && editor->currentOperation != OPERATION_MENU){
            editor->currentPlaceTool++;
            if (editor->currentPlaceTool > PLACE_TOOL_BUCKET){
                editor->currentPlaceTool = 0;
            }
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
                case OPERATION_MENU: currentMode = "editor menu"; break;
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

            // show current tool
            {   // display type
                const char* currentTool;

                switch (editor->currentPlaceTool) {
                    case PLACE_TOOL_PENCIL: currentTool = "pencil"; break;
                    case PLACE_TOOL_BUCKET: currentTool = "fill"; break;
                }
                
                textF("tool %s", 1080, 158, currentTool);
            }
            
        }

        // resize display
        if (editor->currentOperation == OPERATION_RESIZE){
            textF("current w %d h %d", 1000, 8, editor->level->width, editor->level->height);
            textF("new w %d h %d", 1000, 32, editor->newWidth, editor->newHeight);
        }


        // view mode
        {
            const char* viewMode;

            switch (editor->viewMode) {
                case LEVEL_DRAW_EDITOR: viewMode = "editor"; break;
                case LEVEL_DRAW_EDITOR_BACKGROUND: viewMode = "background"; break;
                case LEVEL_DRAW_EDITOR_TILES: viewMode = "tiles"; break;
                case LEVEL_DRAW_GAME: viewMode = "preview"; break;
                case LEVEL_DRAW_CAMERA_VIEW: viewMode = "camera"; break;
            }

            textF("viewmode %s", 8, 690, viewMode);
        }
        
    }

    // editor menu
    {
        if (editor->currentOperation == OPERATION_EDIT && IsKeyDown(KEY_M)){
            editor->currentOperation = OPERATION_MENU;
        }


        if (editor->currentOperation == OPERATION_MENU){
            
            drawUiIntSelector(&editor->level->songId, 4, 500, 200, "bg music id", editor->currentSelectedOption == 0);
            drawUiIntSelector(&editor->level->waterType, 3, 500, 232, "water type", editor->currentSelectedOption == 1);

            // level name
            {
                textF("name %s", 500, 264, editor->level->name);

                if (editor->currentSelectedOption == 2){
                    int currentIndex = strLength(editor->level->name);
                    if (IsKeyPressed(KEY_BACKSPACE) && currentIndex != 0){
                        editor->level->name[currentIndex - 1] = 0;
                    }else if (currentIndex < LEVEL_NAME_LENGTH){
                        for (int i = 0; i <= 25; i++){
                            if (IsKeyPressed(KEY_A + i)){
                                editor->level->name[currentIndex] = i + 'a';
                            }
                        }
                    }

                    
                }
            }

            editorMenuButton(editor, "save level", 3, 500, 296, &saveLevelButton);
            editorMenuButton(editor, "reset level", 4, 500, 328, &resetLevelButton);
            editorMenuButton(editor, "exit menu", 5, 500, 360, &exitMenuButton);

            // switch current index 
            if (IsKeyPressed(KEY_DOWN)){
                editor->currentSelectedOption = (editor->currentSelectedOption + 1) % MENU_ITEMS;
            }

            if (IsKeyPressed(KEY_UP)){
                editor->currentSelectedOption--;
                if (editor->currentSelectedOption < 0){
                    editor->currentSelectedOption = MENU_ITEMS - 1;
                }
            }

            // draw cursor
            drawS(7, 450, 200 + editor->currentSelectedOption * 32, 2.0f, LAYER_STATIC_UI);

        }
    }
}

void exitMenuButton(LevelEditor* editor){
    editor->currentOperation = OPERATION_EDIT;
}

void saveLevelButton(LevelEditor* editor){
    saveLevel(editor->level);
    exitMenuButton(editor);
}

void resetLevelButton(LevelEditor* editor){
    resetLevel(editor->level);
}
void editorMenuButton(LevelEditor* editor, const char* text, int index, int x, int y, void (*useFunc)(LevelEditor* editor)){
    textF(text, x, y);
    if (editor->currentSelectedOption == index && IsKeyPressed(KEY_ENTER)){
        useFunc(editor);
    }
}


void drawUiIntSelector(char* value, int maxValue, int x, int y, const char* textName, bool isSelected){
    textF("%s %d", x, y, textName, *value);
    
    if (!isSelected){
        return;
    }
    // update value
    if (IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT)){
        (*value)--;
        if (*value < 0){
            *value = maxValue - 1;
        }
    }
    else if (IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT)){
        *value = (*value + 1) % maxValue;
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
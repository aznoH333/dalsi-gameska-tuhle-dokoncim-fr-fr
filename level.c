#include "level.h"
#include "gframework.h"
#include "gutil.h"
#include "entities.h"
#include "gcollections.h"
#include "spritedata.h"

#define MIN_LEVEL_FILE_SIZE 4
#define LEVEL_DATA_START 6 + LEVEL_NAME_LENGTH

unsigned char** readTileData(Level* lvl, char* fileData, int index);
void writeTileData(Level* lvl, char* fileData, int index, unsigned char** tileData);

Level* loadLevel(const char* levePath){
    File* f = initFile(levePath);


    if (f->contentsLength < MIN_LEVEL_FILE_SIZE){
        gLog(LOG_ERR, "Invalid file : %s", levePath);
    }

    Level* out = malloc(sizeof(Level));
    out->levelFile = f;
    // read level header
    {
        char w[2] = {f->contents[0], f->contents[1]};
        out->width = parseStrToInt(w, 2);
        char h[2] = {f->contents[2], f->contents[3]};
        out->height = parseStrToInt(h, 2);

        // read water
        out->waterType = f->contents[4];
        out->songId = f->contents[5];

        // read level name
        for (int i = 0; i < LEVEL_NAME_LENGTH; i++){
            out->name[i] = f->contents[6 + i];
        }
    }

    gLog(LOG_DBG, "level info x[%d] y[%d]", out->width, out->height);
    
    if (f->contentsLength < MIN_LEVEL_FILE_SIZE + out->width + out->height){
        gLog(LOG_ERR, "Level data corrupted %s", levePath);
    }
    
    
    // read tile data
    {
        out->tiles = readTileData(out, out->levelFile->contents, LEVEL_DATA_START);
        out->background = readTileData(out, out->levelFile->contents, LEVEL_DATA_START + (out->width * out->height));
    }



    int markersStart = (out->width * out->height * 2) + LEVEL_DATA_START;
    {
        out->entityeMarkers = initVector();
        
        // read markers
        for (int i = markersStart; i < f->contentsLength; i+=6){
            vectorPush(out->entityeMarkers, initEntityMarker(f->contents, i));
        }
    }


    return out;
}

void saveLevel(Level* lvl){
    gLog(LOG_INF, "Saving level to file %s", lvl->levelFile->filePath);

    // construct file contents
    char newContents[MAX_FILE_SIZE];
    int contentsIndex = 0;

    {
        // write header
        writeIntAsChar(newContents, lvl->width, 2, 0);
        writeIntAsChar(newContents, lvl->height, 2, 2);
        newContents[4] = lvl->waterType;
        newContents[5] = lvl->songId;
        // save name
        for (int i = 0; i < LEVEL_NAME_LENGTH; i++){
            newContents[6 + i] = lvl->name[i];
        }

        contentsIndex = LEVEL_DATA_START;
    }

    {
        /*
        // write tiles
        for (int x = 0; x < lvl->width; x++){
            for (int y = 0; y < lvl->height; y++){
                newContents[LEVEL_DATA_START + y + (x * lvl->height)] = lvl->tiles[x][y];
            }
        }
        contentsIndex = LEVEL_DATA_START + (lvl->width * lvl->height);*/

        writeTileData(lvl, newContents, LEVEL_DATA_START, lvl->tiles);
        writeTileData(lvl, newContents, LEVEL_DATA_START + (lvl->width * lvl->height), lvl->background);
        contentsIndex = LEVEL_DATA_START + (lvl->width * lvl->height * 2);
        gLog(LOG_DBG, "level info | tiles start [%d] | background start [%d] | entities start [%d]", LEVEL_DATA_START, LEVEL_DATA_START + (lvl->width * lvl->height), LEVEL_DATA_START + (lvl->width * lvl->height * 2));
        gLog(LOG_DBG, "misc info width [%d] height [%d]", lvl->width, lvl->height);
    }

    {
        // save markers
        for (int i = 0; i < lvl->entityeMarkers->elementCount; i++){
            saveEntityMarker(vectorGet(lvl->entityeMarkers, i), newContents, contentsIndex);
            contentsIndex += 6;
        }
    }

    {
        // set file contents
        char* contetns = malloc(sizeof(char) * contentsIndex);
        // copy contents
        for (int i = 0; i < contentsIndex; i++){
            contetns[i] = newContents[i];
        }
        setFileContents(lvl->levelFile, contetns, contentsIndex);
    }

    

    saveFile(lvl->levelFile);
}

void unloadLevel(Level* level){
    unloadFile(level->levelFile);
    for (int i = 0; i < level->width;i++){
        free(level->tiles[i]);
    }

    for (int i = 0; i < level->width;i++){
        free(level->background[i]);
    }

    for (int i = 0; i < level->entityeMarkers->elementCount; i++){
        disposeEntityMarker(vectorGet(level->entityeMarkers, i));
    }

    vectorFree(level->entityeMarkers);

    free(level->tiles);
    free(level->background);
    free(level);
}

void resetLevel(Level* lvl){
    resizeLevel(lvl, 10, 10);
    for (int x = 0; x < lvl->width; x++){
        for (int y = 0; y < lvl->height; y++){
            lvl->background[x][y] = 0;
            lvl->tiles[x][y] = 0;
        }
    }
    vectorClear(lvl->entityeMarkers);

    lvl->songId = 0;
    lvl->waterType = 0;
    
    for (int i = 0; i < LEVEL_NAME_LENGTH; i++){
        lvl->name[i] = 0;
    }
}


void resizeLevelTileData(Level* level, int newWidth, int newHeight, unsigned char*** tileData){
    // init new contents    
    unsigned char** newLevelContent = malloc(sizeof(char*) * newWidth);
    for (int i = 0; i < newWidth; i++){
        newLevelContent[i] = malloc(sizeof(char*) * newHeight);
    }
    // copy old contents
    for (int x = 0; x < newWidth; x++){
        for (int y = 0; y < newHeight; y++){
            if (x < level->width && y < level->height){
                newLevelContent[x][y] = (*tileData)[x][y];
            }else {
                // else fill with air
                newLevelContent[x][y] = 0;
            }
        }
    }

    // free old contents
    for (int i = 0; i < level->width;i++){
        free((*tileData)[i]);
    }
    free(*tileData);

    // remove out of bounds markers
    for (int i = 0; i < level->entityeMarkers->elementCount; i++){
        EntityMarker* m = vectorGet(level->entityeMarkers, i);

        if (!checkBoxCollisions(m->x, m->y, 1, 1, 0, 0, newWidth, newHeight)){
            vectorRemove(level->entityeMarkers, i);
            i--;
        }
    }

    *tileData = newLevelContent;
}


void resizeLevel(Level* level, int newWidth, int newHeight){
    if(newWidth < 1 || newHeight < 1){
        return;
    }
    
    resizeLevelTileData(level, newWidth, newHeight, &level->tiles);
    resizeLevelTileData(level, newWidth, newHeight, &level->background);

    level->width = newWidth;
    level->height = newHeight;
}

unsigned char** readTileData(Level* lvl ,char* fileData, int index){
    unsigned char** tiles;
    tiles = malloc(sizeof(char*) * lvl->width);

    for (int x = 0; x < lvl->width; x++){
        tiles[x] = malloc(sizeof(char) * lvl->height);
        for (int y = 0; y < lvl->height; y++){
            tiles[x][y] = fileData[(x * lvl->height) + y + index];
        }
    }

    return tiles;
}

void writeTileData(Level* lvl, char* fileData, int index, unsigned char** tileData){
    for (int x = 0; x < lvl->width; x++){
        for (int y = 0; y < lvl->height; y++){
            fileData[index + y + (x * lvl->height)] = tileData[x][y];
        }
    }
}


#define ANIMATED_WATERFALL_SPRITE_ID 141

void drawTile(int spriteId, int x, int y, int layer){
    draw(SPRITE_START_TILES - 1 + spriteId 
    + ((spriteId == ANIMATED_WATERFALL_SPRITE_ID) * ((getGlobalTimer() / 3) % 3)) // the shities hack you have ever seen
    , x * 16, y * 16, layer);
}

// this sucks
// try to make this simpler
const Color MARKER_COLOR = {255, 255, 255, 200};
void drawLevel(Level* lvl, int drawType){
    if (lvl == 0){
        gLog(LOG_ERR, "Attempted to draw unloaded level");
    }


    bool drawForeGround = drawType != LEVEL_DRAW_EDITOR_BACKGROUND;
    bool highlightBlacks = drawType != LEVEL_DRAW_GAME && drawType != LEVEL_DRAW_CAMERA_VIEW;
    bool drawBackGround = drawType != LEVEL_DRAW_EDITOR_TILES;
    bool doCameraPreview = drawType == LEVEL_DRAW_CAMERA_VIEW;
    
    for (int x = 0; x < lvl->width; x++){
        for (int y = 0; y < lvl->height; y++){
            
            
            // fore ground
            {
                unsigned char tile = lvl->tiles[x][y];
                
                if (tile != 0){
                    if (drawForeGround){
                        if (tile != 1 || !highlightBlacks){
                            drawTile(tile, x, y, LAYER_WORLD);
                        }else {
                            draw(SPRITE_VIS_TOGGLE, x * 16, y * 16, LAYER_WORLD);
                        }
                    }else {
                        draw(SPRITE_VIS_TOGGLE + 1, x * 16, y * 16, LAYER_WORLD);
                    }
                }
            }
            
            // background
            {
                unsigned char tile = lvl->background[x][y];
                if (tile != 0){
                    if (drawBackGround){
                        if (tile != 1 || !highlightBlacks){
                            drawTile(tile, x, y, LAYER_BACKGROUND);
                        }else {
                            draw(SPRITE_VIS_TOGGLE, x * 16, y * 16, LAYER_BACKGROUND);
                        }
                    }else {
                        draw(SPRITE_VIS_TOGGLE + 1, x * 16, y * 16, LAYER_BACKGROUND);
                    }
                }
            }
        }
    }
    

    {// draw camera previews
        if (doCameraPreview){
            for (int i = 0; i < lvl->entityeMarkers->elementCount; i++){
                EntityMarker* marker = vectorGet(lvl->entityeMarkers, i);

                if (marker->id == 2 || marker->id == 3){
                    // draw frame
                    for (int x = 0; x < SCREEN_WIDTH / DEFAULT_CAMERA_ZOOM; x += 16){
                        for (int y = 0; y < SCREEN_HEIGHT / DEFAULT_CAMERA_ZOOM; y += 16){
                            drawC(8, marker->x * 16 + x, marker->y * 16 + y, MARKER_COLOR, LAYER_UI);
                            drawC(SPRITE_START_MARKERS + marker->id, marker->x * 16, marker->y * 16, MARKER_COLOR, LAYER_OBJECTS);
                        }
                    }
                }
            }
        }
    }


    {
        // draw entities
        if (!highlightBlacks){
            return;
        }
        // draw markers
        for (int i = 0; i < lvl->entityeMarkers->elementCount; i++){
            EntityMarker* marker = vectorGet(lvl->entityeMarkers, i);
            drawC(SPRITE_START_MARKERS + marker->id, marker->x * 16, marker->y * 16, MARKER_COLOR, LAYER_OBJECTS);
        }
    }
    
}
char getTileAt(Level* lvl, int x, int y){
    if (x < 0 || y < 0 || x >= lvl->width || y >= lvl->height){
        return 0;
    }
    return lvl->tiles[x][y];
}



bool collidesWithLevel(Level* lvl, int x, int y, int w, int h){
    for (int ix = x; ix <= x + w; ix += min(w, 16)){
        for (int iy = y; iy <= y + h; iy += min(h, 16)){
            if (getTileAt(lvl, floor(ix / 16.0f), floor(iy / 16.0f)) > 0){
                return true;
            }
        }
    }
    return false;
}

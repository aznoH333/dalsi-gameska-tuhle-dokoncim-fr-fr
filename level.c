#include "level.h"
#include "gframework.h"
#include "gutil.h"
#include "entities.h"
#include "gvector.h"
#include "spritedata.h"

#define MIN_LEVEL_FILE_SIZE 4
#define LEVEL_DATA_START 4

char** readTileData(Level* lvl, char* fileData, int index);
void writeTileData(Level* lvl, char* fileData, int index, char** tileData);

Level* loadLevel(const char* levePath){
    File* f = initFile(levePath);

    if (f->contentsLength < MIN_LEVEL_FILE_SIZE){
        gLog(LOG_ERROR, "Invalid file : %s", levePath);
    }

    Level* out = malloc(sizeof(Level));
    out->levelFile = f;
    // read level header
    {
        char w[4] = {f->contents[0], f->contents[1]};
        out->width = parseStrToInt(w, 2);
        char h[4] = {f->contents[2], f->contents[3]};
        out->height = parseStrToInt(h, 2);
    }

    
    if (f->contentsLength < MIN_LEVEL_FILE_SIZE + out->width + out->height){
        gLog(LOG_ERROR, "Level data corrupted %s", levePath);
    }
    
    
    // read tile data
    {
        /*
        out->tiles = malloc(sizeof(char*) * out->width);

        for (int x = 0; x < out->width; x++){
            out->tiles[x] = malloc(sizeof(char) * out->height);
            for (int y = 0; y < out->height; y++){
                out->tiles[x][y] = f->contents[(x * out->height) + y + LEVEL_DATA_START];
            }
        }*/

        out->tiles = readTileData(out, out->levelFile->contents, LEVEL_DATA_START);
        out->background = readTileData(out, out->levelFile->contents, LEVEL_DATA_START + (out->width * out->height));
    }



    int markersStart = (out->width * out->height * 2) + LEVEL_DATA_START;
    {
        out->entityeMarkers = initVector();
        
        // read markers
        for (int i = markersStart; i < f->contentsLength; i+=6){
            vectorPush(&out->entityeMarkers, initEntityMarker(f->contents, i));
        }
    }

    return out;
}

void saveLevel(Level* lvl){
    gLog(LOG_INFO, "Saving level to file %s", lvl->levelFile->filePath);

    // construct file contents
    char newContents[MAX_FILE_SIZE];
    int contentsIndex = 0;

    {
        // write header
        writeIntAsChar(newContents, lvl->width, 2, 0);
        writeIntAsChar(newContents, lvl->height, 2, 2);
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

    }

    {
        // save markers
        for (int i = 0; i < lvl->entityeMarkers.elementCount; i++){
            saveEntityMarker(vectorGet(&lvl->entityeMarkers, i), newContents, contentsIndex);
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

    for (int i = 0; i < level->entityeMarkers.elementCount; i++){
        disposeEntityMarker(vectorGet(&level->entityeMarkers, i));
    }

    vectorFree(&level->entityeMarkers);

    free(level->tiles);
    free(level->background);
    free(level);
}


void resizeLevelTileData(Level* level, int newWidth, int newHeight, char*** tileData){
    // init new contents    
    char** newLevelContent = malloc(sizeof(char*) * newWidth);
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

char** readTileData(Level* lvl ,char* fileData, int index){
    char** tiles;
    tiles = malloc(sizeof(char*) * lvl->width);

    for (int x = 0; x < lvl->width; x++){
        tiles[x] = malloc(sizeof(char) * lvl->height);
        for (int y = 0; y < lvl->height; y++){
            tiles[x][y] = fileData[(x * lvl->height) + y + index];
        }
    }

    return tiles;
}

void writeTileData(Level* lvl, char* fileData, int index, char** tileData){
    for (int x = 0; x < lvl->width; x++){
        for (int y = 0; y < lvl->height; y++){
            fileData[index + y + (x * lvl->height)] = tileData[x][y];
        }
    }
}


// this sucks
// try to make this simpler
const Color MARKER_COLOR = {255, 255, 255, 200};
void drawLevel(Level* lvl, int drawType){
    
    bool drawForeGround = drawType != LEVEL_DRAW_EDITOR_BACKGROUND;
    bool highlightBlacks = drawType != LEVEL_DRAW_GAME;
    bool drawBackGround = drawType != LEVEL_DRAW_EDITOR_TILES;
    
    for (int x = 0; x < lvl->width; x++){
        for (int y = 0; y < lvl->height; y++){
            
            
            // fore ground
            {
                char tile = lvl->tiles[x][y];
                
                if (tile != 0){
                    if (drawForeGround){
                        if (tile != 1 || !highlightBlacks){
                            draw(SPRITE_START_TILES - 1 + tile, x * 16, y * 16, LAYER_WORLD);
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
                char tile = lvl->background[x][y];
                if (tile != 0){
                    if (drawBackGround){
                        if (tile != 1 || !highlightBlacks){
                            draw(SPRITE_START_TILES - 1 + tile, x * 16, y * 16, LAYER_BACKGROUND);
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


    if (!highlightBlacks){
        return;
    }
    // draw markers
    for (int i = 0; i < lvl->entityeMarkers.elementCount; i++){
        EntityMarker* marker = vectorGet(&lvl->entityeMarkers, i);
        drawC(SPRITE_START_MARKERS + marker->id, marker->x * 16, marker->y * 16, MARKER_COLOR, LAYER_OBJECTS);
    }
}
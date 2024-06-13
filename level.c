#ifndef LEVEL 
#define LEVEL 

#include "gfiles.c"
#include "glog.c"
#include "gframework.c"
#include "gutil.c"


struct Level {
    char** tiles;
    int width;
    int height;
    File* levelFile;
};
typedef struct Level Level;

#define MIN_LEVEL_FILE_SIZE 4
#define LEVEL_DATA_START 4

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
        out->tiles = malloc(sizeof(char*) * out->width);

        for (int x = 0; x < out->width; x++){
            out->tiles[x] = malloc(sizeof(char) * out->height);
            for (int y = 0; y < out->height; y++){
                out->tiles[x][y] = f->contents[(x * out->width) + y + LEVEL_DATA_START];
            }
        }
    }

    return out;
}

void unloadLevel(Level* level){
    unloadFile(level->levelFile);
    for (int i = 0; i < level->width;i++){
        free(level->tiles[i]);
    }
    free(level->tiles);
    free(level);
}


void resizeLevel(Level* level, int newWidth, int newHeight){
    if(newWidth < 1 || newHeight < 1){
        return;
    }

    // init new contents    
    char** newLevelContent = malloc(sizeof(char*) * newWidth);
    for (int i = 0; i < newWidth; i++){
        newLevelContent[i] = malloc(sizeof(char*) * newHeight);
    }
    // copy old contents
    for (int x = 0; x < newWidth; x++){
        for (int y = 0; y < newHeight; y++){
            if (x < level->width && y < level->height){
                newLevelContent[x][y] = level->tiles[x][y];
            }else {
                // else fill with air
                newLevelContent[x][y] = 0;
            }
        }
    }

    // free old contents
    for (int i = 0; i < level->width;i++){
        free(level->tiles[i]);
    }
    free(level->tiles);


    // set new contents
    level->tiles = newLevelContent;
    level->width = newWidth;
    level->height = newHeight;
}



#define TILE_SPRITE_START 7

void drawLevel(Level* lvl){
    for (int x = 0; x < lvl->width; x++){
        for (int y = 0; y < lvl->height; y++){
            if (lvl->tiles[x][y] != 0){
                draw(TILE_SPRITE_START + lvl->tiles[x][y], x * 32, y * 32, LAYER_WORLD);
            }
        }
    }
}



#endif
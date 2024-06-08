#ifndef LEVEL 
#define LEVEL 

#include "gfiles.c"
#include "glog.c"
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



#endif
#ifndef G_FILES
#define G_FILES 

#include <stdio.h>
#include <stdlib.h>
#include "gutil.h"


#define MAX_FILE_SIZE 1000000


#define FILE_STATUS_OK 0
#define FILE_STATUS_NOT_FOUND 1

struct File{
    char* contents;
    int contentsLength;
    const char* filePath;
    char loadStatus;
};
typedef struct File File;

File* initFile(const char* filePath);
void setFileContents(File* file, char* newContents, int contentSize);
void saveFile(File* file);
void unloadFile(File* file);



#endif
#ifndef G_FILES
#define G_FILES 

#include <stdio.h>
#include <stdlib.h>
#include "gutil.h"


#define MAX_FILE_SIZE 10000

struct File{
    char* contents;
    int contentsLength;
    const char* filePath;
};
typedef struct File File;

File* initFile(const char* filePath);
void setFileContents(File* file, char* newContents, int contentSize);
void saveFile(File* file);
void unloadFile(File* file);



#endif
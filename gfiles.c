#ifndef G_FILES
#define G_FILES 

#include <stdio.h>
#include <stdlib.h>
#include "glog.c"

#define MAX_FILE_SIZE 10000

struct File{
    char* contents;
    int contentsLength;
    const char* filePath;
};


typedef struct File File;

File* initFile(const char* filePath){
    File* out = malloc(sizeof(File));

    out->filePath = filePath;

    FILE* file = fopen(filePath, "r");

    if (file == NULL){
        // create empty file
        out->contentsLength = 0;
        gLog(LOG_WARN, "%s file not found. Creating new empty file", filePath);

    }else {
        // read file contents
        char temp[MAX_FILE_SIZE];

        int index = 0;
        for(;;){
            if (feof(file)){
                index--; // skip last char if feof
                break;
            }
            
            temp[index] = fgetc(file);
            index++;
            
            
            if (index >= MAX_FILE_SIZE){
                gLog(LOG_ERROR, "File read error. Max size exceeded %d", MAX_FILE_SIZE);
            }
        }

        // copy file contents
        out->contents = malloc(index * sizeof(char));
        for (int i = 0; i < index; i++){
            out->contents[i] = temp[i];
        }
        out->contentsLength = index;

        fclose(file);
    }

    return out;
}

void setFileContents(File* file, char* newContents, int contentSize){
    free(file->contents);
    file->contents = newContents;
    file->contentsLength = contentSize;
}


void saveFile(File* file){
    FILE* f = fopen(file->filePath, "w");
    
    if (f == NULL){
        gLog(LOG_ERROR, "File write error %s", file->filePath);
    }

    for (int i = 0; i < file->contentsLength; i++){
        fprintf(f, "%c", file->contents[i]);
    }

    fclose(f);
}

void unloadFile(File* file){
    free(file->contents);
    free(file);
}

#endif
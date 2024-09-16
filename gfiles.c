#include "gfiles.h"

File* initFile(const char* filePath){
    File* out = malloc(sizeof(File));

    out->filePath = filePath;

    FILE* file = fopen(filePath, "r");

    if (file == NULL){
        // create empty file
        out->contentsLength = 0;
        out->loadStatus = FILE_STATUS_NOT_FOUND;
        out->contents = NULL;
        gLog(LOG_WAR, "%s file not found. Creating new empty file", filePath);

    }else {
        // read file contents
        char temp[MAX_FILE_SIZE];

        int index = 0;
        for(;;){
            
            
            char c = fgetc(file);

            if (c == EOF){
                break;
            }

            temp[index] = c;
            index++;
            
            
            if (index >= MAX_FILE_SIZE){
                gLog(LOG_ERR, "File read error. Max size exceeded %d", MAX_FILE_SIZE);
            }
        }
        gLog(LOG_DBG, "read file size [%d]", index);

        // copy file contents
        out->contents = malloc(index * sizeof(char));
        for (int i = 0; i < index; i++){
            out->contents[i] = temp[i];
        }
        out->contentsLength = index;
        out->loadStatus = FILE_STATUS_OK;

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
        gLog(LOG_ERR, "File write error %s", file->filePath);
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
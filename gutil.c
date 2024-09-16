#include "gutil.h"

#include <limits.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "raylib.h"
#include <dirent.h>
#include <string.h>

//------------------------------------------------------------------------------------
// logging
//------------------------------------------------------------------------------------

#define CURRENT_LOG_LEVEL LOG_DBG
void gLog(int level, const char* str, ...){
    if (level > CURRENT_LOG_LEVEL){
        return;
    }
    
    const char* header;

    va_list args;
    va_start(args, str);

    switch (level) {
        case LOG_INF: header = "[INFO]"; break;
        case LOG_ERR: header = "[ERROR]"; break;
        case LOG_WAR: header = "[WARNING]"; break;
        case LOG_DBG: header = "[DEBUG]"; break;
    }
    
    
    printf("%s ", header);
    vprintf(str, args);
    printf("\n");
    va_end(args);

    if (level == LOG_ERR){
        exit(-1);
    }
}

//------------------------------------------------------------------------------------
// strings
//------------------------------------------------------------------------------------
bool strStartsWith(char* str, char* start){
    int i = 0;
    while (start[i] != 0){
        if (str[i] != start[i]){
            return false;
        }
        i++;
    }
    return true;
}


bool strEndsWith(char* str, char* end){
    int length = strLength(str) - 1;
    int lengthEnd = strLength(end) - 1;

    for (int i = lengthEnd; i > 0; i--){
        if (str[length - i] != end[lengthEnd - i]){
            return false;
        }
    }
    return true;
}

int strLength(const char* str){
    int i = 0;
    while (str[i] != 0){i++;}
    return i;
}

const char* boolToStr(bool value){
    if (value){
        return "true";
    }else {
        return "false";
    }
}

bool strEquals(const char* str1, const char* str2){
    int i = 0;
    while (str1[i] != 0){
        if (str1[i] != str2[i]){
            return false;
        }
        i++;
    }
    return true;
}

char* strConcat(const char* str1, const char* str2){
    int charCount = (strLength(str1) + strLength(str2) + 1);
    char* output = malloc(charCount * sizeof(char));

    int index = 0;
    int length1 = strLength(str1);
    for (int i = 0; i < length1; i++){
        output[index] = str1[i];
        index++;
    }
    int length2 = strLength(str2);
    for (int i = 0; i < length2; i++){
        output[index] = str2[i];
        index++;
    }
    output[index] = 0;

    return output;
}


char* intToStr(int value){
    char* output = malloc(10);
    
    
    // edge case
    if (value == 0){
        output[0] = '0';
        output[1] = 0;
        return output;
    }
    
    // convert to str
    int iteration = 0;
    while(value > 0){
        output[iteration] = value % 10 + '0';
        value = (value - (value % 10)) / 10;
        iteration++;
    }
    output[iteration] = 0;

    // reverse output
    int length = strLength(output) - 1;
    for (int i = 0; i < length >> 1; i++){
        int temp = output[length - i];
        output[length - i] = output[i];
        output[i] = temp;
    }

    return output;
}



void copyCharArray(char* original, char* target, int size){
    for (int i = 0; i < size; i++){
        target[i] = original[i];
    }
}
char* createCharArrayCopy(char* original, int size){
    char* output = malloc(sizeof(char) * size);

    copyCharArray(original, output, size);

    return output;
}

//------------------------------------------------------------------------------------
// misc
//------------------------------------------------------------------------------------


bool checkBoxCollisions(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2){
    return x1 + w1 > x2 &&
           x1 < x2 + w2 &&
           y1 + h1 > y2 &&
           y1 < y2 + h2;
}





//------------------------------------------------------------------------------------
// files
//------------------------------------------------------------------------------------
Vector* getFolderContents(const char* folderPath){
    struct dirent *directoryEntry;
    
    Vector* output = initVector();
    DIR *directory = opendir(folderPath); 
  
    if (directory == NULL)
    { 
        gLog(LOG_ERR, "Failed to open directory [%s]", folderPath);
    }
  
    while ((directoryEntry = readdir(directory)) != NULL) {
        char* fileName = malloc(sizeof(char) * 256);
        fileName = memcpy(fileName, directoryEntry->d_name, 256);

        if (strStartsWith(fileName, ".")){
            free(fileName);
        }else {
            vectorPush(output, fileName);
        }
    }
  
    closedir(directory);
    return output;
}

//------------------------------------------------------------------------------------
// math
//------------------------------------------------------------------------------------
float lerp(float a, float b, float w){
    return a * (1.0 - w) + (b * w);
}

float pythagoras(float x1, float y1, float x2, float y2){
    return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

float dirTowards(int x1, int y1, int x2, int y2){
    return atan2(x1 - x2, y1 - y2);
}

int min(int a, int b){
    if (a < b){
        return a;
    }
    return b;
}

int max(int a, int b){
    if (a > b){
        return a;
    }
    return b;
}

bool isInRange(int value, int min, int max){
    return value >= min && value <= max;
}


float sign(float input){
	if (input == 0){
		return 0;
	}else if (input > 0){
		return 1;
	}
	return -1;
}

int parseStrToInt(char* str, int size){
    int out = 0;
    for (int i = size - 1; i >= 0; i--){
        out += str[i] * max(((size - (i + 1)) * (CHAR_MAX + 1)), 1);
    }
    return out;
}

int boolToSign(bool a){
    return a * 2 - 1;
}

void writeIntAsChar(char* targetStr, int input, int size, int index){
    int temp = input;
    for (int i = size - 1; i >= 0; i--){
        targetStr[i + index] = temp % (CHAR_MAX + 1);
        temp /= (CHAR_MAX + 1);
    }
}

void writeObjectToCharArray(void* object, int size, char* target, int index){
    char* content = (char*)object;

    for (int i = 0; i < size; i++){
        target[i + index] = content[i];
    }
}
void readObjectFromCharArray(void* object, int size, char* target, int index){
    char* content = (char*) object;

    for (int i = 0; i < size; i++){
        content[i] = target[i + index];
    }
}


//------------------------------------------------------------------------------------
// random
//------------------------------------------------------------------------------------
int getRandomInt(int maxValue){
    return rand() % (maxValue + 1);
}

int getRandomIntR(int minValue, int maxValue){
    return getRandomInt(maxValue - minValue) + minValue;
}

#define RANDOM_FLOAT_PRECISION 1000.0f

float getRandomFloat(){
    return getRandomInt(RANDOM_FLOAT_PRECISION) / RANDOM_FLOAT_PRECISION;
}

float getRandomFloatRange(float min, float max){
    return getRandomFloat() * (max - min) + min;
}

bool randomChance(float chance){
    return getRandomFloat() < chance;
}

//------------------------------------------------------------------------------------
// animations
//------------------------------------------------------------------------------------
int getAnimationSprite(int startFrame, int endFrame, int frameDuration, int timer){
    return  (timer / frameDuration) % (endFrame - startFrame + 1) + startFrame;
}


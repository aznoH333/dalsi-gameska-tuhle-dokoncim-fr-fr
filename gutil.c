#ifndef G_UTILS
#define G_UTILS

#include <limits.h>
#include <math.h>
#include <stdlib.h>
#include "glog.c"

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

#endif
#ifndef G_UTILS
#define G_UTILS


#include <stdbool.h> 

// loging
#define LOG_ERR 1
#define LOG_WAR 2
#define LOG_INF 3
void gLog(int level, const char* str, ...);

// rest
bool checkBoxCollisions(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);
float lerp(float a, float b, float w);
float pythagoras(float x1, float y1, float x2, float y2);
float dirTowards(int x1, int y1, int x2, int y2);
int min(int a, int b);
int max(int a, int b);
int boolToSign(bool a);
float sign(float input);
int parseStrToInt(char* str, int size);
void writeIntAsChar(char* targetStr, int input, int size, int index);
int getRandomInt(int maxValue);
int getRandomIntR(int minValue, int maxValue);
float getRandomFloat();
float getRandomFloatRange(float min, float max);
bool randomChance(float chance);

// animations
int getAnimationSprite(int startFrame, int endFrame, int frameDuration, int timer);

#endif
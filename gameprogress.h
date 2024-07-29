#ifndef GAME_PROGRESS
#define GAME_PROGRESS


struct GameProgress{
    int score;
};
typedef struct GameProgress GameProgress;



GameProgress* getGameProgress();
void unloadGameProgress();
void addScore(int x, int y, int ammount);



#endif
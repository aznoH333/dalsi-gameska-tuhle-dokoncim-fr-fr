#ifndef GAME_PROGRESS
#define GAME_PROGRESS


struct GameProgress{
    int score;
    int scoreSizeMultiplier;
};
typedef struct GameProgress GameProgress;



GameProgress* getGameProgress();
void displayPlayerUi();
void unloadGameProgress();
void addScore(int x, int y, int ammount);



#endif
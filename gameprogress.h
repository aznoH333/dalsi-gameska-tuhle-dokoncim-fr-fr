#ifndef GAME_PROGRESS
#define GAME_PROGRESS


struct GameProgress{
    int score;
    int scoreSizeMultiplier;
    char lastPlayedLevel;
    char lastClearedLevel;
};
typedef struct GameProgress GameProgress;



GameProgress* getGameProgress();
void displayPlayerUi();
void unloadGameProgress();
void addScore(int x, int y, int ammount);
void saveGameProgress(GameProgress* this);
void loadGameProgress(GameProgress* this);



#endif
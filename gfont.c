#include "gfont.h" 



Texture2D fontTexture;
int sheetWitdth;
int sheetHeight;
void initFont(){
    fontTexture = LoadTexture("resources/font.png");
    sheetWitdth = fontTexture.width / CHARACTER_SIZE;
    sheetHeight = fontTexture.height / CHARACTER_SIZE;
}

void disposeFont(){
    UnloadTexture(fontTexture);
}



char translateCharToSpriteSheetId(char in){
    if (in >= 'a' && in <= 'z'){
        return in - 'a';
    }else if (in >= '0' && in <= '9'){
        return in - '0' + 25;
    }
    gLog(LOG_ERR, "Font error, character %d not defined", in);
    return 0;
}

void drawCharacter(char character, int x, int y, float scale, Color color, int layer){
    Rectangle src = {(character % sheetHeight) * CHARACTER_SIZE, floor((float)character / (float)sheetWitdth) *
	CHARACTER_SIZE, CHARACTER_SIZE, CHARACTER_SIZE};
	Rectangle dest = {x, y, CHARACTER_SIZE * scale, CHARACTER_SIZE * scale};
	Vector2 origin = {0, 0};
	DrawTexturePro(fontTexture, src, dest, origin, 0, color);
}

void drawText(const char* text, int x, int y, float scale, Color color, int layer){
    int iterator = 0;
    while (text[iterator] != 0) {
        char character = translateCharToSpriteSheetId(text[iterator]);
        
        drawCharacter(character, x + (iterator * CHARACTER_SIZE), y, scale, color, layer);
        
        iterator++;
    }
}

void drawTextF(const char* text, int x, int y, float scale, Color color, int layer, ...){
    va_list args;
    char formatedText[500];
    
    sprintf(formatedText, text, args);

    drawText(formatedText, x, y, scale, color, layer);
}
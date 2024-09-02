#include "gframework.h"

//------------------------------------------------------
// Conf
//------------------------------------------------------

const char* WINDOW_NAME = "gameska";
const int DEFAULT_SPRITE_SIZE = 16;

const int SPRITE_ORIGIN_OFFSET = DEFAULT_SPRITE_SIZE >> 1;
const Color BACKGROUND_COLOR = {22, 7, 18, 255};

int currentScreenWidth = SCREEN_WIDTH;
int currentScreenHeight = SCREEN_HEIGHT;


//------------------------------------------------------
// sprites
//------------------------------------------------------
struct FrameworkSpriteSheet{
	Texture2D spriteSheetTexture;
	int width;
	int height;
	int defaultSpriteSize;
	int originOffset;
};
typedef struct FrameworkSpriteSheet FrameworkSpriteSheet;


FrameworkSpriteSheet mainSpriteSheet;
FrameworkSpriteSheet initSpriteSheet(const char* spriteSheetPath, int defaultSpriteSize){
	FrameworkSpriteSheet out;
	out.spriteSheetTexture = LoadTexture(spriteSheetPath);
	out.width = out.spriteSheetTexture.width / defaultSpriteSize;
	out.height = out.spriteSheetTexture.height / defaultSpriteSize;
	out.defaultSpriteSize = defaultSpriteSize;
	out.originOffset = defaultSpriteSize >> 1;
	return out;
}

void unloadSpriteSheet(FrameworkSpriteSheet spriteSheet){
	UnloadTexture(spriteSheet.spriteSheetTexture);
}




//------------------------------------------------------
// Variables
//------------------------------------------------------
FrameworkSpriteSheet loadedSheet;
FrameworkSpriteSheet fontSheet;
RenderTexture2D renderTexture;
Vector2 actualCameraPos = {0,0};
Camera2D cam;
float scalingFactor;
int renderTextureOffset;
float screenShakeAmmount = 0.0f;
int fTimer = 0;


//------------------------------------------------------
// drawing layers and data
//------------------------------------------------------
struct DrawingData{
	int spriteIndex;
	int x;
	int y;
	float rotation;
	float scale;
	Color c;
	FrameworkSpriteSheet* targetSheet;
	int flip;
};
typedef struct DrawingData DrawingData;


Vector* drawingLayers[LAYER_COUNT];
void initDrawingLayers(){
	for (int i = 0; i < LAYER_COUNT; i++){
		drawingLayers[i] = initVector();
	}
}

void cleanDrawingLayers(){
	for (int i = 0; i < LAYER_COUNT; i++){
		vectorFree(drawingLayers[i]);
	}
}


void insertDrawRequest(int spriteIndex, int x, int y, float rotation, int flip, float scale, Color c, int layer, FrameworkSpriteSheet* targetSheet){
	// init data
	DrawingData* data = malloc(sizeof(DrawingData));
	data->spriteIndex = spriteIndex;
	data->x = x;
	data->y = y;
	data->scale = scale;
	data->rotation = rotation;
	data->c = c;
	data->flip = flip;
	data->targetSheet = targetSheet;


	// push to vector
	vectorPush(drawingLayers[layer], data);
}

void drawSpriteData(DrawingData* data){
	int flip = data->flip % 4;
	bool flipHorizontaly = flip == FLIP_HORIZONTAL || flip == FLIP_BOTH;
	bool flipVerticaly = flip == FLIP_VERTICAL || flip == FLIP_BOTH;
	
	Rectangle src = {
		(data->spriteIndex % data->targetSheet->width) * data->targetSheet->defaultSpriteSize, 
		floor((float)data->spriteIndex / data->targetSheet->width) * data->targetSheet->defaultSpriteSize, 
		data->targetSheet->defaultSpriteSize, 
		data->targetSheet->defaultSpriteSize};

	if (flipHorizontaly){
		src.width *= -1;
	}

	if (flipVerticaly){
		src.height *= -1;
	}
	
	
	
	Rectangle dest = {data->x + data->targetSheet->originOffset, data->y + data->targetSheet->originOffset, data->targetSheet->defaultSpriteSize * data->scale, data->targetSheet->defaultSpriteSize * data->scale};
	
	
	Vector2 origin = {data->targetSheet->originOffset, data->targetSheet->originOffset};
	DrawTexturePro(data->targetSheet->spriteSheetTexture, src, dest, origin, data->rotation, data->c);
}

void drawLayer(int layer){
	for (int i = 0; i < drawingLayers[layer]->elementCount;i++){
		drawSpriteData((DrawingData*)vectorGet(drawingLayers[layer], i));
	}
	vectorClear(drawingLayers[layer]);

}

//------------------------------------------------------
// misc
//------------------------------------------------------
int getGlobalTimer(){
	return fTimer;
}

//------------------------------------------------------
// camera
//------------------------------------------------------
void screenShake(float ammount){
	screenShakeAmmount += ammount;
}

void updateCamera(){
	screenShakeAmmount = fmin(screenShakeAmmount, 10);
	Vector2 vec = {actualCameraPos.x + sin(fTimer) * screenShakeAmmount, actualCameraPos.y + cos(fTimer) * screenShakeAmmount};
	cam.target = vec;

	if (screenShakeAmmount < 0.1f){
		screenShakeAmmount = 0;
	}else {
		screenShakeAmmount *= 0.2f;
	}

}

Vector2 getInWorldMousePosition(){
	Vector2 out = GetMousePosition();
	
	out.x /= cam.zoom;
	out.y /= cam.zoom;


	out.x += cam.target.x;
	out.y += cam.target.y;

	return out;
}

Vector2 getOnScreenMousePosition(){
	return GetMousePosition();
}

void setCameraPos(Vector2 pos){
	actualCameraPos = pos;
}


void addCameraZoom(float zoom){
	cam.zoom += zoom;
}

void setCameraZoom(float zoom){
	cam.zoom = zoom;
}

void resetCameraZoom(){
	cam.zoom = DEFAULT_CAMERA_ZOOM;
}


//------------------------------------------------------
// font
//------------------------------------------------------
#define CHARACTER_SIZE 8
void initFont(){
    fontSheet = initSpriteSheet("resources/font.png", CHARACTER_SIZE);
}

void disposeFont(){
    unloadSpriteSheet(fontSheet);
}



char translateCharToSpriteSheetId(char in){
    if (in >= 'a' && in <= 'z'){
        return in - 'a';
    }else if (in >= '0' && in <= '9'){
        return in - '0' + 26;
    }
    gLog(LOG_ERR, "Font error, character %d not defined", in);
    return 0;
}

void drawText(const char* text, int x, int y, float scale, Color color, int layer){
	int iterator = 0;
    while (text[iterator] != 0) {
        
		// skip drawing spaces
		if (text[iterator] != ' '){
			char character = translateCharToSpriteSheetId(text[iterator]);
        
        	insertDrawRequest(character, x + (iterator * CHARACTER_SIZE * scale), y, 0.0f, FLIP_NONE, scale, color, layer, &fontSheet);
        }
        iterator++;
    }
}

void drawTextF(const char* text, int x, int y, float scale, Color color, int layer, ...){
    va_list args;
	va_start(args, layer);
    char formatedText[500];
    
    vsprintf(formatedText, text, args);

	va_end(args);
    drawText(formatedText, x, y, scale, color, layer);

}

void drawTextVF(const char* text, int x, int y, float scale, Color color, int layer, va_list* args){
	
	char formatedText[500];
	vsprintf(formatedText, text, *args);
	drawText(formatedText, x, y, scale, color, layer);
}

void textF(const char* text, int x, int y, ...){
	va_list args;
	va_start(args, y);

	drawTextVF(text, x, y, 2.0, WHITE, LAYER_STATIC_UI, &args);

	va_end(args);
}


//------------------------------------------------------
// drawing
//------------------------------------------------------
void drawRFSC(int spriteIndex, int x, int y, float rotation, int flip, float scale, Color c, int layer){
	insertDrawRequest(spriteIndex, x, y, rotation, flip, scale, c, layer, &loadedSheet);
}

void drawRSC(int spriteIndex, int x, int y, float rotation, float scale, Color c, int layer){
	insertDrawRequest(spriteIndex, x, y, rotation, FLIP_NONE, scale, c, layer, &loadedSheet);
}

void drawFSC(int spriteIndex, int x, int y, int flip, float scale, Color c, int layer){
	drawRFSC(spriteIndex, x, y, 0.0f, flip, scale, c, layer);
}

void drawFC(int spriteIndex, int x, int y, bool flip, Color c, int layer){
	drawRFSC(spriteIndex, x, y, 0.0f, flip, 1.0f, c, layer);
}

void drawFS(int spriteIndex, int x, int y, bool flip, float scale, int layer){
	drawRFSC(spriteIndex, x, y, 0.0f, flip, scale, WHITE, layer);
}


void drawR(int spriteIndex, int x, int y, float rotation, int layer){
	drawRSC(spriteIndex, x, y, rotation, 1.0f, WHITE, layer);
}

void drawC(int spriteIndex, int x, int y, Color c, int layer){
	drawRSC(spriteIndex, x, y, 0.0f, 1.0f, c, layer);
}

void drawS(int spriteIndex, int x, int y, float scale, int layer){
	drawRSC(spriteIndex, x, y, 0.0f, scale, WHITE, layer);
}

void drawF(int spriteIndex, int x, int y, int flip, int layer){
	drawRFSC(spriteIndex, x, y, 0.0f, flip, 1.0f, WHITE, layer);
}

void draw(int spriteIndex, int x, int y, int layer){	
	drawRSC(spriteIndex, x, y, 0.0f, 1.0f, WHITE, layer);
}


void updateMusic();
void fUpdate(){
	BeginTextureMode(renderTexture);
    BeginMode2D(cam);
	updateCamera();
	fTimer++;
    ClearBackground(BACKGROUND_COLOR);
	updateMusic();
	
	for (int i = 0; i < LAYER_STATIC_UI; i++){
		drawLayer(i);
	}

	EndMode2D();


	drawLayer(LAYER_STATIC_UI);

    EndTextureMode();
    
    BeginDrawing();
    ClearBackground(BLACK);
    Rectangle r = { 0, 0, (float)(renderTexture.texture.width), (float)(-renderTexture.texture.height) };
    Rectangle r2 = { renderTextureOffset, 0, (float)(currentScreenWidth) * scalingFactor, (float)(currentScreenHeight) };
    Vector2 v = {0, 0};
    DrawTexturePro(renderTexture.texture,r,r2,v,0,WHITE);

    EndDrawing();
}

void drawFancyText(const char* text, int x, int y, int scale, Color color){
	int shadowOffset = fmax(scale / 10.0f, 1);
	DrawText(text, x + shadowOffset, y, scale, GRAY);
	DrawText(text, x, y, scale, color);

}

void gfullscreen(){
	
	int tWidth = currentScreenWidth;
	int tHeight = currentScreenHeight;
	
	int monitor = GetCurrentMonitor();

	if (IsWindowFullscreen())
    {
        SetWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
        SetWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);

		currentScreenWidth = SCREEN_WIDTH;
		currentScreenHeight = SCREEN_HEIGHT;
    }
	else
    {
        SetWindowSize(GetMonitorWidth(monitor), GetMonitorHeight(monitor));
        SetWindowSize(GetMonitorWidth(monitor), GetMonitorHeight(monitor));

		currentScreenWidth = GetMonitorWidth(monitor);
		currentScreenHeight = GetMonitorHeight(monitor);
    }
	// update scaling factor
	//scalingFactor = currentScreenWidth /(float)(GetScreenWidth());
	gLog(LOG_INF, "Changing screen resolution from w[%d] h[%d] to w[%d] h[%d]", tWidth, tHeight, currentScreenWidth, currentScreenHeight);

	scalingFactor = 1.0f;// scaling factor should be 1 or the fullscreen bugs out??
	ToggleFullscreen();

}

bool shouldRun = true;
void closeGame(){
	shouldRun = false;
}
bool isGameRunning(){
	return !WindowShouldClose() && shouldRun;
}

//------------------------------------------------------
// sounds
//------------------------------------------------------
Map* soundMap;
void playSound(const char* soundName){
	Sound* s = mapGet(soundMap, soundName);
	SetSoundVolume(*s, 0.4f);
	PlaySound(*s);
	
}

void loadSounds(){
	soundMap = initMap();
	Vector* soundFiles = getFolderContents("./resources/sounds/");

	for (int i = 0; i < soundFiles->elementCount; i++){
		// copy file name
		char* fileName = vectorGet(soundFiles, i);
		char* mapKey = malloc(sizeof(char) * (strLength(fileName) + 1));
		memcpy(mapKey, fileName, sizeof(char) * (strLength(fileName) + 1));
		// copy sound to heap
		char* soundFilePath = strConcat("./resources/sounds/", fileName);
		Sound s = LoadSound(soundFilePath);
		free(soundFilePath);
		Sound* mapValue = malloc(sizeof(Sound));
		memcpy(mapValue, &s, sizeof(s));

		// put in map
		mapPut(soundMap, mapKey, mapValue);
    }
    vectorFree(soundFiles);
}

void unloadSounds(){
	for (int i = 0; i < soundMap->elements->elementCount; i++){
		Sound* s = vectorGet(soundMap->elements, i);
		UnloadSound(*s);
	}
	
	mapFree(soundMap);
}

//------------------------------------------------------
// music
//------------------------------------------------------
Vector* musicList;

#define MUSIC_TRACK_COUNT 1
int currentMusicTrack = -1;
Music* currentMusicTrackPtr = 0;
void playMusic(int songId){
	if (songId > MUSIC_TRACK_COUNT || songId < 0){
		gLog(LOG_ERR, "Music track [%d] not found", songId);
	}
	currentMusicTrack = songId;
	currentMusicTrackPtr = vectorGet(musicList, songId);

	PlayMusicStream(*currentMusicTrackPtr);
}

void loadMusic(){
	musicList = initVector();

	for (int i = 0; i < MUSIC_TRACK_COUNT; i++){
		// construct name
		char* songId = intToStr(i);
		char* songTempPath = strConcat("./resources/music/", songId);
		free(songId);
		char* songPath = strConcat(songTempPath, ".wav");
		free(songTempPath);

		// load song
		Music m = LoadMusicStream(songPath);
		free(songPath);

		// insert into vector
		Music* song = malloc(sizeof(Music));
		memcpy(song, &m, sizeof(Music));

		vectorPush(musicList, song);
	}
}

void unloadMusic(){
	for (int i = 0; i < musicList->elementCount; i++){
		Music* m = vectorGet(musicList, i);
		UnloadMusicStream(*m);
	}

	vectorFree(musicList);
}

void updateMusic(){
	//UpdateMusicStream(*currentMusicTrackPtr);
}



//------------------------------------------------------
// init
//------------------------------------------------------
void initFramework(){
	SetTraceLogLevel(LOG_WARNING);
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_NAME);
	InitAudioDevice();
	SetTargetFPS(60);
	renderTexture = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);
	loadedSheet = initSpriteSheet("resources/spritesheet.png", DEFAULT_SPRITE_SIZE);
	scalingFactor = currentScreenWidth /(float)(GetScreenWidth());
	renderTextureOffset = ((GetScreenWidth()) / 2) - (SCREEN_WIDTH / 2);
	cam.zoom = DEFAULT_CAMERA_ZOOM;
	loadSounds();
	loadMusic();
	initDrawingLayers();
	initFont();
}

//------------------------------------------------------
// dispose
//------------------------------------------------------
void disposeFramework(){
	unloadSpriteSheet(loadedSheet);
	disposeFont();
	UnloadRenderTexture(renderTexture);
	cleanDrawingLayers();
	CloseAudioDevice();
	unloadSounds();
	unloadMusic();
	CloseWindow();
}


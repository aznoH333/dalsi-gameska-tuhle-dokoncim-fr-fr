#ifndef G_FRAMEWORK
#define G_FRAMEWORK

#include "raylib.h"
#include <math.h>
#include <string.h>
#include "gvector.c"

//------------------------------------------------------
// Conf
//------------------------------------------------------
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const char* WINDOW_NAME = "template window";
const int DEFAULT_SPRITE_SIZE = 32;
const float DEFAULT_CAMERA_ZOOM = 2.0f;
const int SPRITE_ORIGIN_OFFSET = DEFAULT_SPRITE_SIZE >> 1;




#define LAYER_BACKGROUND 0
#define LAYER_WORLD 1
#define LAYER_OBJECTS 2
#define LAYER_UI 3
#define LAYER_STATIC_UI 4
#define LAYER_COUNT LAYER_STATIC_UI + 1





//------------------------------------------------------
// sprites
//------------------------------------------------------
struct FrameworkSpriteSheet{
	Texture2D spriteSheetTexture;
	int width;
	int height;

};
typedef struct FrameworkSpriteSheet FrameworkSpriteSheet;

FrameworkSpriteSheet mainSpriteSheet;
FrameworkSpriteSheet initSpriteSheet(){
	FrameworkSpriteSheet out;
	out.spriteSheetTexture = LoadTexture("resources/spritesheet.png");
	out.width = out.spriteSheetTexture.width / DEFAULT_SPRITE_SIZE;
	out.height = out.spriteSheetTexture.height / DEFAULT_SPRITE_SIZE;
	
	return out;
}

void unloadSpriteSheet(FrameworkSpriteSheet spriteSheet){
	UnloadTexture(spriteSheet.spriteSheetTexture);
}




//------------------------------------------------------
// Variables
//------------------------------------------------------
FrameworkSpriteSheet loadedSheet;
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
	Color c;
};
typedef struct DrawingData DrawingData;


Vector* drawingLayers[LAYER_STATIC_UI];
void initDrawingLayers(){
	for (int i = 0; i < LAYER_COUNT; i++){
		drawingLayers[i] = malloc(sizeof(Vector));
		
		Vector v = initVector();

		memcpy(drawingLayers[i], &v, sizeof(Vector));
	}
}

void cleanDrawingLayers(){
	for (int i = 0; i < LAYER_COUNT; i++){
		vectorFree(drawingLayers[i]);
	}
}


void insertDrawRequest(int spriteIndex, int x, int y, float rotation, Color c, int layer){
	// init data
	DrawingData* data = malloc(sizeof(DrawingData));
	data->spriteIndex = spriteIndex;
	data->x = x;
	data->y = y;
	data->rotation = rotation;
	data->c = c;


	// push to vector
	vectorPush(drawingLayers[layer], data);
}

void drawSpriteData(DrawingData* data){
	
	Rectangle src = {(data->spriteIndex % loadedSheet.width) * DEFAULT_SPRITE_SIZE, floor((float)data->spriteIndex / (float)loadedSheet.width) *
	DEFAULT_SPRITE_SIZE, DEFAULT_SPRITE_SIZE, DEFAULT_SPRITE_SIZE};
	Rectangle dest = {data->x + SPRITE_ORIGIN_OFFSET, data->y + SPRITE_ORIGIN_OFFSET, DEFAULT_SPRITE_SIZE, DEFAULT_SPRITE_SIZE};
	Vector2 origin = {SPRITE_ORIGIN_OFFSET, SPRITE_ORIGIN_OFFSET};
	DrawTexturePro(loadedSheet.spriteSheetTexture, src, dest, origin, data->rotation, data->c);
}

void drawLayer(int layer){
	for (int i = 0; i < drawingLayers[layer]->elementCount;i++){
		drawSpriteData((DrawingData*)vectorGet(drawingLayers[layer], i));
	}
	vectorClear(drawingLayers[layer]);
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

void setCameraPos(Vector2 pos){
	actualCameraPos = pos;
}


void addCameraZoom(float zoom){
	cam.zoom += zoom;
}

void resetCameraZoom(){
	cam.zoom = DEFAULT_CAMERA_ZOOM;
}


//------------------------------------------------------
// drawing
//------------------------------------------------------


void drawRCL(int spriteIndex, int x, int y, float rotation, Color c, int layer){
	insertDrawRequest(spriteIndex, x, y, rotation, c, layer);

}


void drawRC(int spriteIndex, int x, int y, float rotation, Color c){
	insertDrawRequest(spriteIndex, x, y, rotation, c, LAYER_OBJECTS);
}

void drawR(int spriteIndex, int x, int y, float rotation){
	drawRC(spriteIndex, x, y, rotation, WHITE);
}

void drawC(int spriteIndex, int x, int y, Color c){
	drawRC(spriteIndex, x, y, 0.0f, c);
}

void draw(int spriteIndex, int x, int y){	
	drawC(spriteIndex, x, y, WHITE);
}




void drawRL(int spriteIndex, int x, int y, float rotation, int layer){
	drawRCL(spriteIndex, x, y, rotation, WHITE, layer);
}

void drawCL(int spriteIndex, int x, int y, Color c, int layer){
	drawRCL(spriteIndex, x, y, 0.0f, c, layer);
}

void drawL(int spriteIndex, int x, int y, int layer){	
	drawCL(spriteIndex, x, y, WHITE, layer);
}


void fUpdate(){
	BeginTextureMode(renderTexture);
    BeginMode2D(cam);
	updateCamera();
	fTimer++;
    ClearBackground(BLACK);
	
	for (int i = 0; i < LAYER_STATIC_UI; i++){
		drawLayer(i);
	}

	EndMode2D();


	drawLayer(LAYER_STATIC_UI);

    EndTextureMode();
    
    BeginDrawing();
    ClearBackground(BLACK);
    Rectangle r = { 0, 0, (float)(renderTexture.texture.width), (float)(-renderTexture.texture.height) };
    Rectangle r2 = { renderTextureOffset, 0, (float)(GetScreenWidth()) * scalingFactor, (float)(GetScreenHeight()) };
    Vector2 v = {0, 0};
    DrawTexturePro(renderTexture.texture,r,r2,v,0,WHITE);

    EndDrawing();
}

void drawFancyText(const char* text, int x, int y, int scale, Color color){
	int shadowOffset = fmax(scale / 10.0f, 1);
	DrawText(text, x + shadowOffset, y, scale, GRAY);
	DrawText(text, x, y, scale, color);

}

//------------------------------------------------------
// init
//------------------------------------------------------
void initFramework(){
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_NAME);
	SetTargetFPS(60);
	renderTexture = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);
	loadedSheet = initSpriteSheet();
	scalingFactor = SCREEN_WIDTH /(float)(GetScreenWidth());
	renderTextureOffset = ((GetScreenWidth()) / 2) - (SCREEN_WIDTH / 2);
	//ToggleFullscreen();
	cam.zoom = DEFAULT_CAMERA_ZOOM;
	initDrawingLayers();
}

//------------------------------------------------------
// dispose
//------------------------------------------------------
void disposeFramework(){
	unloadSpriteSheet(loadedSheet);
	UnloadRenderTexture(renderTexture);
	cleanDrawingLayers();
	CloseWindow();
}


#endif

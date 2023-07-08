#ifndef PIXEL_VIEWER_H
#define PIXEL_VIEWER_H

#include "typedefs.h"
#include "SimpleGameEngine.h"
#include "include/Data/Pixel.h"

class PixelViewer : public SimpleGameEngine {
private:
	LTexturePtr gTexture = nullptr;

	Mix_Music* gMusic = nullptr;
	Mix_Chunk* gChunk = nullptr;

	LButtonPtr testButton = nullptr;
	LTextPtr buttonText = nullptr;

	typedef std::vector<PixelPtr> pixels;
	std::vector<pixels> imagesPixels = std::vector<pixels>();
	std::vector<LWindowPtr> pixelsShowWindows = std::vector<LWindowPtr>();

	virtual void OnStart();
	virtual void OnEvent(SDL_Event& e);
	virtual void OnUpdate(float deltaTime);
	virtual void OnDestroy();
public:
	PixelViewer();
	virtual ~PixelViewer();
};

#endif // PIXEL_VIEWER_H

/*#include <string>
#include "include/Data/Pixel.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <sstream>
#include "LTexture.h"
#include "LButton.h"
#include "LTimer.h"
#include "Dot.h"
#include <vector>
#include "LWindow.h"
#include <fstream>

//The dimensions of the level
const int LEVEL_WIDTH = 1280;
const int LEVEL_HEIGHT = 960;

const int SCREEN_FPS = 60;
const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;

const int TOTAL_DATA = 10;

//Maximum number of supported recording devices
const int MAX_RECORDING_DEVICES = 10;

//Maximum recording time
const int MAX_RECORDING_SECONDS = 5;

//Maximum recording time plus padding
const int RECORDING_BUFFER_SECONDS = MAX_RECORDING_SECONDS + 1;

//The various recording actions we can take
enum RecordingState
{
	SELECTING_DEVICE,
	STOPPED,
	RECORDING,
	RECORDED,
	PLAYBACK,
	ERROR
};

//Recording/playback callbacks
void audioRecordingCallback(void* userdata, Uint8* stream, int len);
void audioPlaybackCallback(void* userdata, Uint8* stream, int len);

const int TOTAL_WINDOWS = 1;

//Tile constants
const int TILE_WIDTH = 80;
const int TILE_HEIGHT = 80;
const int TOTAL_TILES = 192;
const int TOTAL_TILE_SPRITES = 12;

//The different tile sprites
const int TILE_RED = 0;
const int TILE_GREEN = 1;
const int TILE_BLUE = 2;
const int TILE_CENTER = 3;
const int TILE_TOP = 4;
const int TILE_TOPRIGHT = 5;
const int TILE_RIGHT = 6;
const int TILE_BOTTOMRIGHT = 7;
const int TILE_BOTTOM = 8;
const int TILE_BOTTOMLEFT = 9;
const int TILE_LEFT = 10;
const int TILE_TOPLEFT = 11;

class PixelViewer
{
	//Our custom windows
	LWindow gWindows[TOTAL_WINDOWS];

	//Our custom window
	LWindow gWindow;

	//Display data
	int gTotalDisplays = 0;
	SDL_Rect* gDisplayBounds = NULL;

	//The window renderer
	SDL_Renderer* gRenderer = NULL;

	//Globally used font
	TTF_Font* gFont = NULL;

	//Scene textures
	LTexture gSceneTexture;
	LTexture gDotTexture;
	LTexture gRedTexture;
	LTexture gGreenTexture;
	LTexture gBlueTexture;
	LTexture gShimmerTexture;

	//The text textures that specify recording device names
	LTexture gDeviceTextures[MAX_RECORDING_DEVICES];

	//Data points
	Sint32 gData[TOTAL_DATA];

	unsigned int screen_width;
	unsigned int screen_height;

	Pixel* pixels;
public:
	PixelViewer();
	~PixelViewer();
	void SetPixels(const Pixel* newPixels, unsigned int length);
	void LoadMedia();
	void Init(std::string window_name, unsigned int screen_width, unsigned int screen_height);
	void EventLoop();
	void Close();
	//Circle/Circle collision detector
	static bool checkCollision(Circle& a, Circle& b);

	//Circle/Box collision detector
	static bool checkCollision(Circle& a, SDL_Rect& b);

	//Calculates distance squared between two points
	static double distanceSquared(int x1, int y1, int x2, int y2);
};

#endif*/
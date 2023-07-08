#include "SimpleGameEngine.h"

SimpleGameEngine::SimpleGameEngine() {
}

SimpleGameEngine::~SimpleGameEngine() 
{
	OnDestroy();
}

void SimpleGameEngine::Init(const std::string& mainWindowTitle, unsigned int mainWindowWidth, unsigned int mainWindowHeight)
{
	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
	}
	else
	{
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		//Get number of displays
		unsigned int totalDisplays = SDL_GetNumVideoDisplays();
		if (totalDisplays < 2)
		{
			printf("Warning: Only one display connected!");
		}

		//Get bounds of each display
		for (int i = 0; i < totalDisplays; ++i)
		{
			gDisplayBounds.push_back(new SDL_Rect());
			SDL_GetDisplayBounds(i, gDisplayBounds[i]);
		}

		//Create window
		mainWindow = loadWindow(mainWindowTitle, mainWindowWidth, mainWindowHeight);
		if (!mainWindow->init())
		{
			printf("Window 0 could not be created!\n");
		}
		else
		{
			//Initialize PNG loading
			int imgFlags = IMG_INIT_PNG;
			if (!(IMG_Init(imgFlags) & imgFlags))
			{
				printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
			}

			//Initialize SDL_ttf
			if (TTF_Init() == -1)
			{
				printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
			}

			//Initialize SDL_mixer
			if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
			{
				printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
			}

			gRenderer = mainWindow->getRenderer();
		}
	}

	EventLoop();
}

LWindowPtr SimpleGameEngine::loadWindow(const std::string& windowTitle, unsigned int windowWidth, unsigned int windowHeight)
{
	LWindowPtr window = std::make_shared<LWindow>(windowTitle, windowWidth, windowHeight);
	loadedWindows.push_back(window);
	return window;
}

LTexturePtr SimpleGameEngine::loadTexture(const std::string& path)
{
	LTexturePtr texture = std::make_shared<LTexture>();
	if (!texture->loadFromFile(path, gRenderer))
	{
		printf("Failed to load texture!\n");
		return nullptr;
	}
	else {
		loadedTextures.push_back(texture);
		return texture;
	}
}

Mix_Music* SimpleGameEngine::loadMusic(const std::string& path)
{
	Mix_Music* music = Mix_LoadMUS(path.c_str());
	if (music == nullptr) {
		printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
	}
	else {
		loadedMusics.push_back(music);
	}
	return music;
}

Mix_Chunk* SimpleGameEngine::loadChunk(const std::string& path)
{
	Mix_Chunk* chunk = Mix_LoadWAV(path.c_str());
	if (chunk == nullptr)
	{
		printf("Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError());
	}
	else {
		loadedChunks.push_back(chunk);
	}
	return chunk;
}

LButtonPtr SimpleGameEngine::addButton(const Point2D& pos, int w, int h, const LTexturePtr& defaultSprite, const LTexturePtr& mouseOutSprite, const LTexturePtr& mouseOverSprite, const LTexturePtr& mouseDownSprite, const LTexturePtr& mouseUpSprite, const Point2DPtr& parent)
{
	LButtonPtr button = std::make_shared<LButton>(pos, w, h, defaultSprite, mouseOutSprite, mouseOverSprite, mouseDownSprite, mouseUpSprite, parent);
	addedButtons.push_back(button);
	return button;
}

LTextPtr SimpleGameEngine::addText(const Point2D& pos, const std::string& text, unsigned int size, SDL_Color color, const std::string& fontPath, const Point2DPtr& parent)
{
	LTextPtr newText = std::make_shared<LText>(pos, text, color, fontPath, size, gRenderer, parent);
	addedTexts.push_back(newText);
	return newText;
}

void SimpleGameEngine::OnStart()
{
}

void SimpleGameEngine::OnUpdate(float deltaTime)
{
}

void SimpleGameEngine::OnEvent(SDL_Event& e)
{
}

void SimpleGameEngine::OnDestroy()
{
	//Open data for writing
	/*SDL_RWops* file = SDL_RWFromFile("./Tutorial/33_file_reading_and_writing/nums.bin", "w+b");
	if (file != NULL)
	{
		//Save data
		for (int i = 0; i < TOTAL_DATA; ++i)
		{
			SDL_RWwrite(file, &gData[i], sizeof(Sint32), 1);
		}

		//Close file handler
		SDL_RWclose(file);
	}
	else
	{
		printf("Error: Unable to save file! %s\n", SDL_GetError());
	}*/

	//Free loaded images
	for (auto& tex : loadedTextures) {
		tex->free();
	}
	loadedTextures.clear();

	//Free texts
	for (auto& text : addedTexts) {
		text->free();
	}
	addedTexts.clear();

	//Free musics
	for (auto& music : loadedMusics) {
		Mix_FreeMusic(music);
	}
	loadedMusics.clear();

	//Free chunks
	for (auto& chunk : loadedChunks) {
		Mix_FreeChunk(chunk);
	}
	loadedChunks.clear();

	//Destroy windows
	for (auto& window : loadedWindows)
	{
		window->free();
	}
	loadedWindows.clear();

	//Free playback audio
	/*if (gRecordingBuffer != NULL)
	{
		delete[] gRecordingBuffer;
		gRecordingBuffer = NULL;
	}*/

	//Quit SDL subsystems
	Mix_Quit();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

void SimpleGameEngine::EventLoop()
{
	bool quit = false;

	SDL_Event e;

	/*std::vector<LTexture> window1Textures;
	std::vector<TextureRenderData> window1TexturesRenderData;*/

	//Set the default recording state
	//RecordingState currentState = SELECTING_DEVICE;

	//Audio device IDs
	//SDL_AudioDeviceID recordingDeviceId = 0;
	//SDL_AudioDeviceID playbackDeviceId = 0;

	//The dot that will be moving around on the screen
	//Dot dot(gRedTexture, gGreenTexture, gBlueTexture);

	OnStart();
	LTimer updateTimer;
	updateTimer.start();

	//While application is running
	while (!quit)
	{
		//Handle events on queue
		while (SDL_PollEvent(&e) != 0)
		{
			//User requests quit
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}

			//Handle input for the dot
			//dot.handleEvent(e);

			OnEvent(e);

			bool allWindowsHided = true;
			for (auto& window : loadedWindows) {
				window->handleEvent(e);
				if (window->isShown() == true) {
					allWindowsHided = false;
				}
			}
			for (auto& button : addedButtons) {
				button->handleEvent(e);
			}

			if (allWindowsHided) {
				quit = true;
			}
		}

		OnUpdate(updateTimer.getTime());
		updateTimer.start();

		for (auto window : loadedWindows) {
			window->update();
		}
	}
}

const LWindowPtr& SimpleGameEngine::getMainWindow() const
{
	return mainWindow;
}

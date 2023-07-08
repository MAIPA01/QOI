#pragma once
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <vector>
#include <string>
#include "LButton.h"
#include "LTexture.h"
#include "LWindow.h"
#include "LTimer.h"
#include "LText.h"
#include "typedefs.h"

class SimpleGameEngine
{
private:
	//general Renderer
	SDL_Renderer* gRenderer = nullptr;

	//Our custom window
	LWindowPtr mainWindow = nullptr;

	//Display data
	std::vector<SDL_Rect*> gDisplayBounds = std::vector<SDL_Rect*>();

	// Loaded Data
	std::vector<LWindowPtr> loadedWindows = std::vector<LWindowPtr>();
	std::vector<LTexturePtr> loadedTextures = std::vector<LTexturePtr>();
	std::vector<Mix_Music*> loadedMusics = std::vector<Mix_Music*>();
	std::vector<Mix_Chunk*> loadedChunks = std::vector<Mix_Chunk*>();

	// Added Objects
	std::vector<LButtonPtr> addedButtons = std::vector<LButtonPtr>();
	std::vector<LTextPtr> addedTexts = std::vector<LTextPtr>();

	virtual void OnStart();
	virtual void OnUpdate(float deltaTime);
	virtual void OnEvent(SDL_Event& e);
	virtual void OnDestroy();

	void EventLoop();

public:
	SimpleGameEngine();
	virtual ~SimpleGameEngine();
	void Init(const std::string& mainWindowTitle, unsigned int mainWindowWidth, unsigned int mainWindowHeight);
	LWindowPtr loadWindow(const std::string& windowTitle, unsigned int windowWidth, unsigned int windowHeight);
	LTexturePtr loadTexture(const std::string& path);
	Mix_Music* loadMusic(const std::string& path);
	Mix_Chunk* loadChunk(const std::string& path);

	LButtonPtr addButton(const Point2D& pos, int w, int h, const LTexturePtr& defaultSprite, const LTexturePtr& mouseOutSprite = nullptr, const LTexturePtr& mouseOverSprite = nullptr, const LTexturePtr& mouseDownSprite = nullptr, const LTexturePtr& mouseUpSprite = nullptr, const Point2DPtr& parent = nullptr);
	LTextPtr addText(const Point2D& pos, const std::string& text, unsigned int size, SDL_Color color, const std::string& fontPath, const Point2DPtr& parent = nullptr);

	const LWindowPtr& getMainWindow() const;
};


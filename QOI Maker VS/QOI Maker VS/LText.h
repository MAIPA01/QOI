#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include "LTexture.h"
#include "typedefs.h"

class LText
{
private:
	Point2DPtr parent;

	std::string text;
	TTF_Font* font;
	unsigned int size;
	LTexturePtr textTexture;
	SDL_Color textColor;
	Point2DPtr position;
public:
	LText(const Point2D& position, const std::string& text, SDL_Color textColor, const std::string& fontPath, unsigned int size, SDL_Renderer* renderer, const Point2DPtr& parent = nullptr);

	void setParent(const Point2DPtr& parent);
	void setPosition(const Point2D& pos);

	virtual ~LText();

	void free();

	const Point2DPtr getParent() const;
	LTexturePtr getTexture() const;
	unsigned int getWidth() const;
	unsigned int getHeight() const;
	const Point2DPtr getPosition() const;
};


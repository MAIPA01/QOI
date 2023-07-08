#include "LText.h"

LText::LText(const Point2D& position, const std::string& text, SDL_Color textColor, const std::string& fontPath, unsigned int size, SDL_Renderer* renderer, const Point2DPtr& parent) : position(std::make_shared<Point2D>(position)), parent(parent), text(text), size(size)
{
	textTexture = std::make_shared<LTexture>();

	TTF_Font* font = TTF_OpenFont(fontPath.c_str(), size);
	if (font == nullptr)
	{
		printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else {
		textTexture->loadFromRenderedText(text, textColor, font, renderer);
	}
}

void LText::setParent(const Point2DPtr& parent)
{
	this->parent = parent;
}

void LText::setPosition(const Point2D& pos) 
{
	position->x = pos.x;
	position->y = pos.y;
}

LText::~LText()
{
	free();
}

void LText::free()
{
	TTF_CloseFont(font);
}

const Point2DPtr LText::getParent() const
{
	return parent;
}

LTexturePtr LText::getTexture() const
{
	return textTexture;
}

unsigned int LText::getWidth() const
{
	return textTexture->getWidth();
}

unsigned int LText::getHeight() const
{
	return textTexture->getHeight();
}

const Point2DPtr LText::getPosition() const
{
	return position;
}

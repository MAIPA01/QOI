#pragma once

#include <memory>
#include <functional>

// Simple Game Engine:
class SimpleGameEngine;
typedef std::shared_ptr<SimpleGameEngine> SimpleGameEnginePtr;

class LWindow;
typedef std::shared_ptr<LWindow> LWindowPtr;

class LCamera;
typedef std::shared_ptr<LCamera> LCameraPtr;

class LButton;
typedef std::shared_ptr<LButton> LButtonPtr;
typedef std::function<void(void)> LButtonCall;

class LTexture;
typedef std::shared_ptr<LTexture> LTexturePtr;
struct TextureRenderData;

class LTimer;
typedef std::shared_ptr<LTimer> LTimerPtr;

class LText;
typedef std::shared_ptr<LText> LTextPtr;

struct Point2D {
	float x = 0;
	float y = 0;
};
typedef std::shared_ptr<Point2D> Point2DPtr;

struct Point3D {
	float x = 0;
	float y = 0;
	float z = 0;
};
typedef std::shared_ptr<Point3D> Point3DPtr;

// QOI:
class Pixel;
typedef std::shared_ptr<Pixel> PixelPtr;
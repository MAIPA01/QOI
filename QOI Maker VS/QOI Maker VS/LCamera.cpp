#include "LCamera.h"

LCamera::LCamera(const Point3D& pos, float near, float far, const Point3DPtr& parent) : pos(std::make_shared<Point3D>(pos)), near(near), far(far), parent(parent)
{
}

LCamera::LCamera() : pos(std::make_shared<Point3D>(Point3D())), near(0.1f), far(1000.0f), parent(nullptr)
{
}

LCamera::~LCamera()
{
}

const Point3DPtr LCamera::getPosition() const
{
	return pos;
}

const Point3DPtr LCamera::getParent() const
{
	return parent;
}

void LCamera::MoveTo(const Point3D& pos)
{
	this->pos->x = pos.x;
	this->pos->y = pos.y;
}

void LCamera::setParent(const Point3DPtr& parent)
{
	this->parent = parent;
}

float LCamera::getNear() const
{
	return near;
}

float LCamera::getFar() const
{
	return far;
}

void LCamera::setNear(float near)
{
	this->near = near;
}

void LCamera::setFar(float far)
{
	this->far = far;
}

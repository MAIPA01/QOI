#pragma once

#include "typedefs.h"

class LCamera
{
private:
	Point3DPtr parent;

	Point3DPtr pos;

	float near;
	float far;

public:
	LCamera();
	LCamera(const Point3D& pos, float near, float far, const Point3DPtr& parent = nullptr);
	~LCamera();

	const Point3DPtr getPosition() const;
	const Point3DPtr getParent() const;

	void MoveTo(const Point3D& pos);
	void setParent(const Point3DPtr& parent);

	float getNear() const;
	float getFar() const;

	void setNear(float near);
	void setFar(float far);
};


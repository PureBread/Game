#pragma once

#include <OrthographicCamera.h>

class VCam : public OrthographicCamera{
public:
	VCam(float _width, float _height/*, float _x, float _y*/);

	virtual void update(Step * _step) override;
private:
	float width, height;
};
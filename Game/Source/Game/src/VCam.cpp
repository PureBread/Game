#pragma once

#include <VCam.h>
#include <Easing.h>

VCam::VCam(float _width, float _height/*, float _x, float _y*/) :
	width(_width),
	height(_height),
	OrthographicCamera(-_width/2, _width/2, -_height/2, _height/2, -1000, 1000)
{
	// childTransform->translate(_x, _y, 0, false);
}


void VCam::update(Step * _step){
	float t = fmod(_step->time, 6);
	if (t > 3){
		t = 6 - t;
	}

	width = Easing::easeInOutBack(t, 192/4*3, -192 / 32, 3);
	height = Easing::easeInOutBack(t, 108/4*3, -108 / 32, 3);


	left = -width * 0.5f;
	right = width * 0.5f;
	bottom = - height * 0.5f;
	top = height * 0.5f;

	OrthographicCamera::update(_step);
}
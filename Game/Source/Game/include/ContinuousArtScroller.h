#pragma once

#include <MeshEntity.h>

/*
Two planes which move in sync such that a sequence of images can be displayed on them without ever seeing a break.
*/
class ContinuousArtScroller : public Entity{
public:
	// file directory which contains the separated art pieces
	std::string fileDir;
	// speed at which the scroller moves
	float speed;
	// currently loaded image
	unsigned long int imageId;

	MeshEntity * plane1;
	MeshEntity * plane2;
	MeshEntity * frontPlane;
	MeshEntity * backPlane;

	ContinuousArtScroller(std::string _fileDir, float _speed, Shader * _shader);

	void loadTexOntoPlane(unsigned long int _texId, MeshEntity * _plane);
	void cycle(signed long int _delta);
	void swapPlanes();
};
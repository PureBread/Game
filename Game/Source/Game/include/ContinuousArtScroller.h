#pragma once

#include <MeshEntity.h>
class Texture;

/*
Two planes which move in sync such that a sequence of images can be displayed on them without ever seeing a break.
*/
class ContinuousArtScroller : public Entity{
public:
	// file directory which contains the separated art pieces
	std::string fileDir;
	// speed at which the scroller moves
	float speed;
	// current progress along the scroller
	float progress;

	// currently loaded image
	signed long int imageId;

	MeshEntity * plane1;
	MeshEntity * plane2;
	MeshEntity * frontPlane;
	MeshEntity * backPlane;

	std::vector<Texture *> images;
	unsigned long int imageCount;

	ContinuousArtScroller(std::string _fileDir, float _speed, Shader * _shader);
	~ContinuousArtScroller();

	void loadTexOntoPlane(unsigned long int _texId, MeshEntity * _plane);
	void cycle(signed long int _delta);
	void swapPlanes();

	virtual void update(Step * _step) override;
};
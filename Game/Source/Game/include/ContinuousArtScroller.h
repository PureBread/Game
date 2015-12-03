#pragma once

#include <ArtLayer.h>
#include <MeshEntity.h>
#include <shader/ComponentShaderBase.h>
class Texture;

/*
Two planes which move in sync such that a sequence of images can be displayed on them without ever seeing a break.
*/
class ContinuousArtScroller : public ArtLayer{
public:

	// file directory which contains the separated art pieces
	std::string fileDir;
	// current progress along the scroller
	float progress;

	// currently loaded image
	signed long int imageId;

	std::vector<MeshEntity *> planes;
	unsigned long int frontPlane;
	unsigned long int backPlane;
	const unsigned long int numPlanes;

	std::vector<Texture *> images;
	unsigned long int imageCount;

	ContinuousArtScroller(std::string _fileDir, ComponentShaderBase * _shader);
	~ContinuousArtScroller();

	void loadTexOntoPlane(signed long int _texId, MeshEntity * _plane);
	void cycle(signed long int _delta);

	virtual void update(Step * _step) override;
};
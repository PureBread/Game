#pragma once

#include <ContinuousArtScroller.h>
#include <MeshFactory.h>
#include <MeshInterface.h>
#include <Texture.h>
#include <sstream>
#include <MY_ResourceManager.h>

ContinuousArtScroller::ContinuousArtScroller(std::string _fileDir, float _speed, Shader * _shader) :
	fileDir(_fileDir),
	speed(_speed),
	plane1(new MeshEntity(MeshFactory::getPlaneMesh(), _shader)),
	plane2(new MeshEntity(MeshFactory::getPlaneMesh(), _shader)),
	imageId(1)
{
	backPlane = plane1;
	frontPlane = plane2;
	loadTexOntoPlane(1, plane1);
	loadTexOntoPlane(2, plane2);

	plane1->mesh->scaleModeMag = GL_NEAREST;
	plane1->mesh->scaleModeMin = GL_NEAREST;
	plane2->mesh->scaleModeMag = GL_NEAREST;
	plane2->mesh->scaleModeMin = GL_NEAREST;

	childTransform->addChild(backPlane);
	childTransform->addChild(frontPlane)->translate(1,0,0);
}

void ContinuousArtScroller::cycle(signed long int _delta){
	imageId += _delta;

	if (_delta > 0){
		loadTexOntoPlane(imageId+1, backPlane);
		backPlane->firstParent()->translate(2, 0, 0);
		swapPlanes();
	}else{
		loadTexOntoPlane(imageId, frontPlane);
		frontPlane->firstParent()->translate(-2, 0, 0);
		swapPlanes();
	}
}

void ContinuousArtScroller::loadTexOntoPlane(unsigned long int _texId, MeshEntity * _plane){
	std::stringstream src;
	src << "assets/textures/" << fileDir << "/" << _texId << ".png";
	Texture * texture = new Texture(src.str(), true, true);
	texture->load();
	while(_plane->mesh->textures.size() > 0){
		_plane->mesh->removeTextureAt(0);
	}
	_plane->mesh->pushTexture2D(texture/*MY_ResourceManager::scenario->getTexture("DEFAULT")->texture*/);
}

void ContinuousArtScroller::swapPlanes(){
	MeshEntity * t = frontPlane;
	frontPlane = backPlane;
	backPlane = t;
}
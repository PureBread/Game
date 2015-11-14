#pragma once

#include <ContinuousArtScroller.h>
#include <MeshFactory.h>
#include <MeshInterface.h>
#include <Texture.h>
#include <sstream>
#include <MY_ResourceManager.h>

ContinuousArtScroller::ContinuousArtScroller(std::string _fileDir, ComponentShaderBase * _shader) :
	ArtLayer(dynamic_cast<ShaderComponentReplace *>(_shader->getComponentAt(2))),
	fileDir(_fileDir),
	plane1(new MeshEntity(MeshFactory::getPlaneMesh(), _shader)),
	plane2(new MeshEntity(MeshFactory::getPlaneMesh(), _shader)),
	imageId(1),
	imageCount(0),
	progress(0)
{
	while (true){
		++imageCount;
		std::stringstream src;
		src << "assets/textures/" << fileDir << "/";
		if(imageCount < 10){
			src << "0";
		}
		src << imageCount << ".png";
		if (!FileUtils::fileExists(src.str())){
			break;
		}
		Texture * texture = new Texture(src.str(), true, false);
		texture->loadImageData();
		images.push_back(texture);
	}



	backPlane = plane1;
	frontPlane = plane2;
	loadTexOntoPlane(1, plane1);
	loadTexOntoPlane(2, plane2);

	MeshInterface * m = MeshFactory::getPlaneMesh();
	m->configureDefaultVertexAttributes(_shader);
	m->pushTexture2D(MY_ResourceManager::scenario->defaultTexture->texture);
	GL_NEAREST;

	backPlane->meshTransform->addChild(m)->translate(0, -1, 0);
	frontPlane->meshTransform->addChild(m)->translate(0, -1, 0);

	m->referenceCount += 2;

	plane1->mesh->scaleModeMag = plane1->mesh->scaleModeMin = plane2->mesh->scaleModeMag = plane2->mesh->scaleModeMin = m->scaleModeMag = m->scaleModeMin = GL_NEAREST;

	childTransform->addChild(backPlane);
	childTransform->addChild(frontPlane)->translate(1,0,0);
}

ContinuousArtScroller::~ContinuousArtScroller(){
	// remove any active textures so they aren't delete twice
	while(frontPlane->mesh->textures.size() > 0){
		frontPlane->mesh->textures.at(0)->unload();
		frontPlane->mesh->removeTextureAt(0);
	}while(backPlane->mesh->textures.size() > 0){
		backPlane->mesh->textures.at(0)->unload();
		backPlane->mesh->removeTextureAt(0);
	}

	// delete textures
	while (images.size() > 0){
		delete images.back();
		images.pop_back();
	}
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
	while(_plane->mesh->textures.size() > 0){
		_plane->mesh->textures.at(0)->unload();
		_plane->mesh->removeTextureAt(0);
	}
	if (_texId < images.size()){
		_plane->mesh->pushTexture2D(images.at(_texId));
	}else{
		_plane->mesh->pushTexture2D(MY_ResourceManager::scenario->getTexture("DEFAULT")->texture);
	}
	_plane->mesh->textures.at(0)->load();
}

void ContinuousArtScroller::swapPlanes(){
	MeshEntity * t = frontPlane;
	frontPlane = backPlane;
	backPlane = t;
}

void ContinuousArtScroller::update(Step * _step){
	//childTransform->translate(progress*speed, 0, 0, false);

	//std::cout << progress* speed << ": " << imageId << std::endl;

	while (imageId - progress < -1){
		cycle(1);
	}while (imageId - progress > 1){
		cycle(-1);
	}

	Entity::update(_step);
}
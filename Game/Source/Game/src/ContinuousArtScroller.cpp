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
	imageId(1),
	imageCount(0),
	progress(0),
	numPlanes(4)
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
	
	MeshInterface * m = MeshFactory::getPlaneMesh();
	m->configureDefaultVertexAttributes(_shader);
	m->pushTexture2D(MY_ResourceManager::scenario->defaultTexture->texture);
	m->scaleModeMag = m->scaleModeMin = GL_NEAREST;

	for(unsigned long int i = 0; i < numPlanes; ++i){
		MeshEntity * plane = new MeshEntity(MeshFactory::getPlaneMesh(), _shader);
		loadTexOntoPlane(i+1, plane);

		plane->mesh->scaleModeMag = plane->mesh->scaleModeMin = GL_NEAREST;
		plane->meshTransform->addChild(m)->translate(0, -1, 0);
		childTransform->addChild(plane)->translate(i,0,0);
		planes.push_back(plane);
	}
	m->referenceCount += numPlanes;

	backPlane = 0;
	frontPlane = numPlanes-1;
}

ContinuousArtScroller::~ContinuousArtScroller(){
	// remove any active textures so they aren't delete twice
	for(MeshEntity * e : planes){
		while(e->mesh->textures.size() > 0){
			e->mesh->textures.at(0)->unload();
			e->mesh->removeTextureAt(0);
		}
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
		loadTexOntoPlane(imageId+(numPlanes/2+1), planes.at(backPlane));
		planes.at(backPlane)->firstParent()->translate(numPlanes, 0, 0);

		frontPlane = backPlane;
		++backPlane;
		if(backPlane >= numPlanes){
			backPlane = 0;
		}
	}else{
		loadTexOntoPlane(imageId, planes.at(frontPlane));
		planes.at(frontPlane)->firstParent()->translate(-(signed long int)numPlanes, 0, 0);
		
		backPlane = frontPlane;
		--frontPlane;
		if(frontPlane >= numPlanes){
			frontPlane = numPlanes-1;
		}
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

void ContinuousArtScroller::update(Step * _step){
	while (imageId - progress < -(signed long int)numPlanes/2){
		cycle(1);
	}while (imageId - progress > -(signed long int)numPlanes/2+1){
		cycle(-1);
	}

	Entity::update(_step);
}
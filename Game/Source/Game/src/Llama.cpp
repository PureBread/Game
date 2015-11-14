#pragma once

#include <Llama.h>
#include <Sprite.h>
#include <Texture.h>
#include <MeshInterface.h>
#include <Easing.h>

Llama::Llama(Shader * _shader) :
	llama(new Sprite(_shader)),
	isHopping(false),
	hopSpeed(1.f),
	hopDuration(1.f),
	hopLength(5.f),
	hopHeight(2.f),
	currHopTime(0.f)
{
	Texture * texture = new Texture("assets/textures/PureBread-logo.png", true, false);
	texture->load();
	llama->mesh->pushTexture2D(texture);
	childTransform->addChild(llama);
}

Llama::~Llama(){
}

void Llama::update(Step * _step){
	Entity::update(_step);

	if (isHopping){
		currHopTime += _step->deltaTime;
		
		float ly = currHopTime / hopDuration <= 0.5 ? Easing::easeOutSine(currHopTime, 0, hopHeight, hopDuration) : Easing::easeOutBounce(currHopTime, hopHeight, -hopHeight, hopDuration);

		// part of stride/hop
		childTransform->translate(deltaY * _step->deltaTime * hopSpeed, deltaY * _step->deltaTime * hopSpeed, 0);
		llama->childTransform->translate(0, ly, 0);

		if (currHopTime >= hopDuration){
			isHopping = false;
		}
	}
}

void Llama::addTarget(glm::vec2 _target){
	targets.push_back(_target);
}

void Llama::setPath(glm::vec2 _startPos, glm::vec2 _targetPos){
	pathP1 = _startPos;
	pathP2 = _targetPos;

	deltaX = pathP2.x - pathP1.x;
	deltaY = pathP2.y - pathP1.y;
}

void Llama::hop(){
	if (!isHopping){
		isHopping = true;
		hopStartPos = glm::vec2(childTransform->getTranslationVector().x, childTransform->getTranslationVector().y);
		hopEndPos = getPointOnPath(hopLength);
		currHopTime = 0.f;
	}
}

glm::vec2 Llama::getPointOnPath(float _distance){
	// get vector
	glm::vec2 v = glm::vec2(pathP2.x - pathP1.x, pathP2.y - pathP1.y);
	v = glm::normalize(v);

	glm::vec2 p = pathP1 + v * hopLength;

	return p;
}


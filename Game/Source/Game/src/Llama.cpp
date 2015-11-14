#pragma once

#include <Llama.h>
#include <Sprite.h>
#include <Texture.h>
#include <MeshInterface.h>
#include <Easing.h>

Llama::Llama(Shader * _shader):
	llama(new Sprite(_shader)),
	isHopping(false),
	hopDuration(1000.f),
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
		
		float x = Easing::linear(currHopTime, hopStartPos.x, hopEndPos.x - hopStartPos.x, hopDuration);
		float y = Easing::linear(currHopTime, hopStartPos.y, hopEndPos.y - hopStartPos.y, hopDuration);
		float ly = currHopTime / hopDuration <= 0.5 ? Easing::easeOutSine(currHopTime, 0, hopHeight, hopDuration) : Easing::easeOutBounce(currHopTime, hopHeight, 0, hopDuration);

		if (x <= 1.f){
			// part of stride/hop
			childTransform->translate(x, 0, 0);
			llama->childTransform->translate(0, ly, 0);
		}else {
			isHopping = false;
		}
	}
}

void Llama::setPath(glm::vec2 _startPos, glm::vec2 _targetPos){
	
	pathP2 = _targetPos;

	if (isHopping){
		hopEndPos = getPointOnPath(hopLength);
	}

	pathP1 = _startPos;
	
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


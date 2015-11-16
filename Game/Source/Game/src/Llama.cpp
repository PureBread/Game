#pragma once

#include <Llama.h>
#include <Sprite.h>
#include <Texture.h>
#include <MeshInterface.h>
#include <Easing.h>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

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

	for (unsigned long int i = 0; i < llama->mesh->vertices.size(); ++i){
		llama->mesh->vertices.at(i).y += 0.5f;
	}
}

Llama::~Llama(){
}

void Llama::update(Step * _step){
	Entity::update(_step);

	if (isHopping){
		currHopTime += _step->deltaTime;

		if (targets.size() > 0 && childTransform->getTranslationVector().x >= targets.at(0).x){
			targets.erase(targets.begin());

			if (targets.size() > 0){
				setPath(glm::vec2(childTransform->getTranslationVector().x, childTransform->getTranslationVector().y), targets.at(0));
			}
		}
		
		float ly = currHopTime / hopDuration <= 0.5 ? Easing::easeOutCubic(currHopTime, 0, hopHeight, hopDuration/2) : Easing::easeOutBounce(currHopTime - hopDuration/2, hopHeight, -hopHeight, hopDuration/2);

		childTransform->translate(deltaX * _step->deltaTime * hopSpeed, deltaY * _step->deltaTime * hopSpeed, 0);
		llama->childTransform->translate(0, ly, 0, false);

		if (currHopTime >= hopDuration){
			isHopping = false;
		}
	}
}

void Llama::addTarget(glm::vec2 _target){
	targets.push_back(_target);

	if (targets.size() == 1){
		setPath(glm::vec2(childTransform->getTranslationVector().x, childTransform->getTranslationVector().y), targets.at(0));
	}
}

void Llama::setPath(glm::vec2 _startPos, glm::vec2 _targetPos){
	pathP1 = _startPos;
	pathP2 = _targetPos;

	deltaX = pathP2.x - pathP1.x;
	deltaY = pathP2.y - pathP1.y;
	glm::vec2 slope = pathP2 - pathP1;
	glm::vec2 angleDirection = glm::rotate(slope, 90.f);
	float angle = glm::angle(glm::vec2(1.f, 0), angleDirection) - 90.f;

	llama->childTransform->setOrientation(glm::angleAxis(angle, glm::vec3(0, 0, 1)));
	
	int i = 0;
}

void Llama::hop(){
	if (!isHopping && targets.size() > 0){
		isHopping = true;
		//hopStartPos = glm::vec2(childTransform->getTranslationVector().x, childTransform->getTranslationVector().y);
		//hopEndPos = getPointOnPath(hopLength);
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


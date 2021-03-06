#pragma once

#include <Llama.h>
#include <Sprite.h>
#include <Texture.h>
#include <MeshInterface.h>
#include <Easing.h>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <MY_ResourceManager.h>
#include <SpeechBubble.h>
#include <NumberUtils.h>

Llama::Llama(Shader * _shader) :
	llama(new Sprite(_shader)),
	center(new Transform()),
	head(new Transform()),
	speechBubble(nullptr),
	isHopping(false),
	hopSpeed(0.1f),
	hopSpeedMultiplier(1.f),
	hopDuration(0.2f),
	hopHeight(0.5f),
	hopHeightMultiplier(1.3f),
	currHopTime(0.f),
	standDuration(0.2f),
	angle1(0),
	angle2(0),
	offset(0),
	leader(nullptr)
{
	llama->mesh->pushTexture2D(MY_ResourceManager::scenario->getTexture("LLAMA")->texture);
	childTransform->addChild(llama);
	llama->mesh->scaleModeMag = llama->mesh->scaleModeMin = GL_NEAREST;

	llama->meshTransform->addChild(center)->translate(0, 0.5, 0);
	llama->meshTransform->addChild(head)->translate(0.5, 1, 0);

	// move the llama's mesh up so that the origin is aligned with the base
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

		if (currHopTime <= hopDuration){
			if (targets.size() > 0 && childTransform->getTranslationVector().x >= targets.at(0).x){
				targets.erase(targets.begin());

				if (targets.size() > 0){
					setPath(glm::vec2(childTransform->getTranslationVector().x, childTransform->getTranslationVector().y), targets.at(0));
				}
			}
		
			// moving
			glm::vec2 v(deltaX, deltaY);
			v = glm::normalize(v);
			childTransform->translate(v.x * _step->deltaTime * hopSpeed, v.y * _step->deltaTime * hopSpeed, 0);
		
			// jumping
			float ly = currHopTime / hopDuration <= 0.5 ? Easing::easeOutCubic(currHopTime, 0, hopHeight, hopDuration / 2) : Easing::easeInCubic(currHopTime - hopDuration / 2, hopHeight, -hopHeight, hopDuration / 2);
			llama->childTransform->translate(0, ly, 0, false);
		}

		if (currHopTime >= hopDuration + standDuration){
			isHopping = false;
		}
	}
	
	// rotate llama to align it with the tangent of the path, interpolated linearly between the two points making up the segment on which it currently stands
	float stepProgress = (childTransform->getTranslationVector().x - pathP1.x) / deltaX;
	llama->childTransform->setOrientation(glm::angleAxis(angle1 + (angle2-angle1) * std::min(stepProgress, 1.f), glm::vec3(0, 0, 1)));
}

void Llama::addTarget(glm::vec2 _target){
	targets.push_back(_target);

	if (targets.size() == 1){
		setPath(glm::vec2(childTransform->getTranslationVector().x, childTransform->getTranslationVector().y), targets.at(0));
	}
}

void Llama::setPath(glm::vec2 _startPos, glm::vec2 _targetPos){
	angle1 = angle2;
	pathP1 = _startPos;
	pathP2 = _targetPos;

	deltaX = pathP2.x - pathP1.x;
	deltaY = pathP2.y - pathP1.y;
	glm::vec2 slope = pathP2 - pathP1;
	angle2 = glm::angle(glm::normalize(slope), glm::vec2(1.f, 0));
	
	// if the llama is going down, flip the angle to match
	if(deltaY < 0){
		angle2 *= -1;
	}
}

void Llama::hop(){
	if (!isHopping && targets.size() > 0 && (leader == nullptr || (childTransform->getTranslationVector().x < leader->childTransform->getTranslationVector().x - offset))){
		isHopping = true;
		currHopTime = 0;
	
		glm::vec2 v1 = glm::vec2(childTransform->getTranslationVector().x, childTransform->getTranslationVector().y);
		float d = 0;
		for(auto v : targets){
			if (leader != nullptr && v.x > leader->childTransform->getTranslationVector().x - offset){
				d += glm::distance(glm::vec2(leader->childTransform->getTranslationVector().x - offset, leader->childTransform->getTranslationVector().y), v1);
				break;
			} else{
				d += glm::distance(v, v1);
			}
			
			v1 = v;
		}
		hopSpeed = hopHeight = d;

		//hopSpeed *= hopSpeedMultiplier;
		hopHeight *= hopHeightMultiplier;

		standDuration = (float)sweet::NumberUtils::randomInt(0, 100) / 100.f;
	}
}

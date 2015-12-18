#pragma once

#include <SpeechBubble.h>
#include <Llama.h>
#include <Camera.h>
#include <MY_ResourceManager.h>
#include <NumberUtils.h>

SpeechBubble::SpeechBubble(Shader * _shader, Camera * _cam, Llama * _llama) :
	Sprite(_shader),
	cam(_cam),
	llama(_llama),
	length(2.f),
	duration(0.f)
{
	mesh->setScaleMode(GL_NEAREST);

	// move the llama's mesh up so that the origin is aligned with the base
	for (unsigned long int i = 0; i < mesh->vertices.size(); ++i){
		mesh->vertices.at(i).y += 0.5f;
	}

	setVisible(false);
}

SpeechBubble::~SpeechBubble(){
}

void SpeechBubble::update(Step * _step){

	if (isVisible()){
		if (duration <= 0){
			setVisible(false);
		} else{
			setPos(llama->head->getWorldPos());
			duration -= _step->getDeltaTime();
		}
	}
}

void SpeechBubble::enable(){
	setVisible(true);
	duration = length;


	// clear old textures
	while(mesh->textureCount() > 0){
		mesh->popTexture2D();
	}
	
	// load new texture
	std::stringstream ss;
	ss << "BUBBLES_" << sweet::NumberUtils::randomInt(1, NUM_BUBBLES);
	Texture * tex = MY_ResourceManager::scenario->getTexture(ss.str())->texture;
	mesh->pushTexture2D(tex);
	
	childTransform->scale(tex->width, tex->height, 1, false);
}

void SpeechBubble::setPos(glm::vec3 _pos){
	glm::vec3 pos = getDistortedScreenPosition(cam, _pos);

	childTransform->translate(pos, false);
}

glm::vec3 SpeechBubble::getDistortedScreenPosition(Camera * _cam, glm::vec3 _pos){
	// llamas world position, plug into cam world to screen pos, compare against mouse.x mouse.y
	glm::uvec2 sd = sweet::getWindowDimensions();
	glm::vec3 pos = _cam->worldToScreen(_pos, sd);
	float r = 50;

	// distort the llamas actual world position with the surface shader's distortion logic!!! lololol
	glm::vec2 uv = glm::vec2(pos.x / sd.x, pos.y / sd.y);
	float xd = (0.75 - abs(uv.x - 0.5) - 0.25);
	float yd = (0.75 - abs(uv.y - 0.5) - 0.25);

	uv.x -= sin((float)sweet::lastTimestamp*0.1001f)*0.1f*xd;
	uv.y -= sin((float)sweet::lastTimestamp*0.0501f)*0.1f*yd;

	pos.x = uv.x * sd.x;
	pos.y = uv.y * sd.y;

	return pos;
}

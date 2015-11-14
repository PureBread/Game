#pragma once

#include <Llama.h>
#include <Texture.h>
#include <MeshInterface.h>
Llama::Llama(Shader * _shader):
	isHopping(false)
{
	Texture * texture = new Texture("assets/textures/PureBread-logo.png", true, false);
	texture->load();
	mesh->pushTexture2D(texture);
}

Llama::~Llama(){
}

void Llama::update(Step * _step){
	Sprite::update(_step);
}

void Llama::hop(glm::vec2 _targetPos){
	
}
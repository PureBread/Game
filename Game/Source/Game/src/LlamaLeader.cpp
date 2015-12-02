#pragma once

#include <LlamaLeader.h>
#include <Sprite.h>
#include <Texture.h>
#include <MeshInterface.h>
#include <MY_ResourceManager.h>

LlamaLeader::LlamaLeader(Shader * _shader) :
	Llama(_shader)
{
	llama->mesh->removeTextureAt(0);
	llama->mesh->pushTexture2D(MY_ResourceManager::scenario->getTexture("LLAMA_LEADER")->texture);

}

LlamaLeader::~LlamaLeader(){
}

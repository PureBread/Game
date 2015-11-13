#pragma once

#include <LevelPath.h>
#include <Texture.h>
#include <sstream>
#include <TextureUtils.h>

LevelPath::LevelPath(std::string _texDir)
{
	Texture * texture = new Texture("assets/textures/" + _texDir, true, false);
	texture->loadImageData();
	vertices = sweet::TextureUtils::getTracedContour(texture);

	int blah = 0;
}

LevelPath::~LevelPath(){
}
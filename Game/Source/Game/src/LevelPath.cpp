#pragma once

#include <LevelPath.h>
#include <Texture.h>
#include <sstream>
#include <TextureUtils.h>

LevelPath::LevelPath(std::string _texDir)
{
	Texture * texture = new Texture("assets/textures/" + _texDir, true, false);
	texture->loadImageData();
	vertices = sweet::TextureUtils::getTracedContour(texture, 0);
	
	unsigned int minX = texture->width, maxX = 0, minY = texture->height;

	// find contour borders
	for (int i = 0; i < vertices.size(); ++i){
		glm::vec2 v = vertices.at(i);
		if (v.x < minX){
			minX = v.x;
		}
		if (v.x > maxX){
			maxX = v.x;
		}
		if (v.y < minY){
			minY = v.y;
		}
	}

	// Save the value of the hightest left border vertex
	int maxY_l = minY;
	for (int i = 0; i < vertices.size(); ++i){
		if (vertices.at(i).x == minX){
			if (vertices.at(i).y > maxY_l){
				maxY_l = vertices.at(i).y;
			}
		}
	}

	// Save the value of the hightest right border vertex
	int maxY_r = minY;
	for (int i = 0; i < vertices.size(); ++i){
		if (vertices.at(i).x == maxX){
			if (vertices.at(i).y > maxY_r){
				maxY_r = vertices.at(i).y;
			}
		}
	}

	// Delete border vertices
	std::vector<glm::vec2>::iterator it = vertices.begin();
	while (it != vertices.end()){

		if ((it->x == minX && it->y < maxY_l) ||
			(it->x == maxX && it->y < maxY_r) ||
			(it->y == minY)){
			it = vertices.erase(it);
		}else{
			++it;
		}
	}

	int blah = 0;
}

LevelPath::~LevelPath(){
}
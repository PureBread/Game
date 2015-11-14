#pragma once

#include <LevelPath.h>
#include <Texture.h>
#include <sstream>
#include <TextureUtils.h>
#include <NumberUtils.h>

LevelPath::LevelPath(std::string _texDir)
{
	Texture * texture = new Texture("assets/textures/" + _texDir, true, false);
	texture->loadImageData();
	vertices = sweet::TextureUtils::getTracedContour(texture, 0);
	
	unsigned int minX = texture->width, maxX = 0, minY = texture->height;

	// find contour borders
	for (unsigned int i = 0; i < vertices.size(); ++i){
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
	for (unsigned int i = 0; i < vertices.size(); ++i){
		if (vertices.at(i).x == minX){
			if (vertices.at(i).y > maxY_l){
				maxY_l = vertices.at(i).y;
			}
		}
	}

	// Save the value of the hightest right border vertex
	int maxY_r = minY;
	for (unsigned int i = 0; i < vertices.size(); ++i){
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

	std::vector<glm::vec2> diagonals;
	// Blah
	for (int i = 0; i < vertices.size(); ++i){
		if (i % 2 == 0){
			diagonals.push_back(vertices.at(i));
		}
	}
	vertices = diagonals;

	// Simplify path
	float threshold = 0.1;
	int spacing = 10;

	if (vertices.size() > spacing * 2){
		std::vector<glm::vec2> simplified;
		simplified.push_back(vertices.front());

		for (int i = spacing; i < vertices.size()-1; i+= spacing){
			glm::vec2 v = vertices.at(i);
			glm::vec2 next = vertices.at(i+spacing < vertices.size() ? i+spacing : vertices.size()-1);
			glm::vec2 prev = vertices.at(i-spacing);

			float slopeNext = (next.y - v.y)/(next.x - v.x);
			float slopePrev = (v.y - prev.y)/(v.x - prev.x);

			float b = abs(slopeNext - slopePrev);
			if (abs(slopeNext - slopePrev) >= threshold){
				simplified.push_back(v);
			}
		}
			
		simplified.push_back(vertices.back());
		vertices = simplified;
	}

	// Normalize Path
	for (int i = 0; i < vertices.size(); ++i){
		vertices.at(i).x = vertices.at(i).x / texture->width;
		vertices.at(i).y = vertices.at(i).y / texture->height;
	}
	
	int blah = 0;
}

LevelPath::~LevelPath(){
}
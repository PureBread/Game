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

	// Simplify path
	// 0 = constant, 1 = increasing, -1 = decreasing
	int smooth = 3;

	if (vertices.size() > 2){
		std::vector<glm::vec2> simplified;
		simplified.push_back(vertices.front());

		int slope = vertices.at(1).y - vertices.at(0).y > 0 ? 1 : vertices.at(1).y - vertices.at(0).y < 0 ? 1 : 0;

		for (int i = 1; i < vertices.size(); ++i){
			glm::vec2 v = vertices.at(i);
			glm::vec2 prev = vertices.at(i - 1);

			int deltaY = v.y - prev.y;
			int prevSlope = slope;

			if (deltaY == 0 && slope != 0){
				slope = 0;
			}else if (deltaY > 0 && slope != 1){
				slope = 1;
			}else if (deltaY < 0 && slope != -1){
				slope = -1;
			}
			
			// If slope direction changes at this point, keep the vertex, else remove it
			if (prevSlope != slope){
				simplified.push_back(prev);
			}
		}

		simplified.push_back(vertices.back());
		vertices = simplified;
	}

	int blah = 0;
}

LevelPath::~LevelPath(){
}
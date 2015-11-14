#pragma once

#include <LevelPath.h>
#include <Texture.h>
#include <sstream>
#include <TextureUtils.h>
#include <NumberUtils.h>
#include <algorithm>

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

	vertices = simplifyVertices(vertices, 0.1, 15);
	
	// Normalize Path
	int scale = std::max(texture->width, texture->height);

	for (int i = 0; i < vertices.size(); ++i){
		vertices.at(i).x = vertices.at(i).x / scale;
		vertices.at(i).y = vertices.at(i).y / scale;
	}
	
	int blah = 0;
}

LevelPath::~LevelPath(){
}

std::vector<glm::vec2> LevelPath::simplifyVertices(std::vector<glm::vec2> _vertices, float _threshold, float _spacing){
	// Simplify path
	if (_vertices.size() > _spacing * 2){
		std::vector<glm::vec2> simplified;

		simplified.push_back(_vertices.front());

		for (int i = _spacing; i < _vertices.size() - 1; i += _spacing){
			glm::vec2 v = _vertices.at(i);
			glm::vec2 next = _vertices.at(i + _spacing < _vertices.size() ? i + _spacing : _vertices.size() - 1);
			glm::vec2 prev = _vertices.at(i - _spacing);

			float slopeNext = (next.y - v.y) / (next.x - v.x);
			float slopePrev = (v.y - prev.y) / (v.x - prev.x);

			float b = abs(slopeNext - slopePrev);
			if (abs(slopeNext - slopePrev) >= _threshold){
				simplified.push_back(v);
			}
		}

		simplified.push_back(_vertices.back());
		return simplified;
	} else{
		return _vertices;
	}
}
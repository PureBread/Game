#pragma once

#include <LevelPath.h>
#include <Texture.h>
#include <sstream>
#include <TextureUtils.h>
#include <NumberUtils.h>
#include <algorithm>
#include<Easing.h>

#include <Sprite.h>
#include <MY_ResourceManager.h>

LevelPath::LevelPath(std::string _texDir):
	idx(0),
	moveThing(nullptr)
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

	if (vertices.size() > 0){
		pos = vertices.at(0);
	}
	
	int blah = 0;
}

LevelPath::~LevelPath(){
}

void LevelPath::setProgress(float _x){
	// idx = target vertex
	// idx-1 = previous target vertex
	
	// update pos according to line equation (with slope)
	if (idx > 0 && idx < vertices.size()){
		pos.x = _x;
		float dX = slope.x;
		float dY = slope.y;
		float s = dX / dY;

		pos.y = slope.y / slope.x * (_x - vertices.at(idx-1).x) + vertices.at(idx - 1).y;
	}

	// update index and snap pos if we've reached a new line segment
	if (vertices.size() > 0 && idx < vertices.size() && _x >= vertices.at(idx).x){
		while (vertices.size() > 0 && idx < vertices.size() && _x >= vertices.at(idx).x){
			++idx;
		}
		
		if (idx < vertices.size()){
			// snap to starting pos of the new line segment
			pos = vertices.at(idx - 1);slope = vertices.at(idx) - vertices.at(idx - 1);
			for (int i = 0; i < llamas.size(); ++i){
				// finalize llamas last target and create a new target
				//llamas.at(i)->
				llamas.at(i)->addTarget(vertices.at(idx));
			}
		}
	}
}

void LevelPath::update(Step * _step){
	Entity::update(_step);
	if (moveThing != nullptr){
		moveThing->childTransform->translate(pos.x, pos.y, 0, false);
	}
	/*
	// Get new line segment slope and add target to llamas
	if (vertices.size() > 0 && idx < vertices.size() && pos.x >= vertices.at(idx).x){
		// first target will be idx = 1
		++idx;
		if (idx < vertices.size()){
			pos = vertices.at(idx-1);
			slope = glm::normalize(vertices.at(idx) - vertices.at(idx-1));
			for (int i = 0; i < llamas.size(); ++i){
				llamas.at(i)->addTarget(vertices.at(idx));
			}
		}
	}
	*/

	// move position
	/*
	if (idx < vertices.size()){
		pos += glm::vec2(slope.x * _step->deltaTime * speed, slope.y * _step->deltaTime * speed);
		std::cout << "idx: " << idx << " posX: " << pos.x << " posY: " << pos.y << std::endl;
	}*/

	// move llamas
	for (int i = 0; i < llamas.size(); ++i){
		llamas.at(i)->hop();
	}
}

void LevelPath::addLlama(Llama * _llama){
	llamas.push_back(_llama);
	_llama->childTransform->translate(glm::vec3(pos.x, pos.y, 0), false);
	childTransform->addChild(_llama);
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

void LevelPath::scaleVertices(float _scale){
	for (int i = 0; i < vertices.size(); ++i){
		vertices.at(i).x = vertices.at(i).x * _scale;
		vertices.at(i).y = vertices.at(i).y * _scale;
	}
}
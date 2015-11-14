#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <Entity.h>

class LevelPath : public Entity {
public:
	std::vector<glm::vec2> vertices;

	LevelPath(std::string _texDir);
	~LevelPath();

	std::vector<glm::vec2> simplifyVertices(std::vector<glm::vec2> _vertices, float _threshold = 0.1, float _spacing = 10);
};
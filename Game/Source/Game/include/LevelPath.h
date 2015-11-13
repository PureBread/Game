#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <string>

class LevelPath {
public:
	std::vector<glm::vec2> vertices;

	LevelPath(std::string _texDir);
	~LevelPath();
};
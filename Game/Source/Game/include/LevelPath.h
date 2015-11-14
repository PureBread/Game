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
};
#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <Entity.h>
#include <Llama.h>

class Sprite;

class LevelPath : public Entity {
private:
	int idx;
	glm::vec2 pos;
	glm::vec2 slope;

	std::vector<Llama *> llamas;

public:
	Sprite * moveThing;
	
	std::vector<glm::vec2> vertices;

	LevelPath(std::string _texDir);
	~LevelPath();

	virtual void update(Step * _step) override;

	void setProgress(float _x);

	void addLlama(Llama * _llama);

	std::vector<glm::vec2> simplifyVertices(std::vector<glm::vec2> _vertices, float _threshold = 0.1, float _spacing = 10);
	void scaleVertices(float _scale);
};
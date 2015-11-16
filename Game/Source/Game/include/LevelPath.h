#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <Entity.h>
#include <Llama.h>
#include <MeshInterface.h>

class LevelPath : public Entity {
private:
	int idx;
	glm::vec2 pos;
	glm::vec2 slope;
	float speed;

	std::vector<Llama *> llamas;

public:
	std::vector<glm::vec2> vertices;

	LevelPath(std::string _texDir);
	~LevelPath();

	virtual void update(Step * _step) override;

	void addLlama(Llama * _llama);

	std::vector<glm::vec2> simplifyVertices(std::vector<glm::vec2> _vertices, float _threshold = 0.1, float _spacing = 10);
	void scaleVertices(float _scale);

	MeshInterface * getMesh();
};
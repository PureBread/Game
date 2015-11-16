#pragma once

#include <Entity.h>

class Sprite;
class Shader;

class Llama : public Entity{
private:
	glm::vec2 pathP1;
	glm::vec2 pathP2;

	std::vector<glm::vec2> targets;

	float deltaX;
	float deltaY;

	float angle1;
	float angle2;

	bool isHopping;
	float hopSpeed;
	float hopDuration;
	float hopLength;
	float hopHeight;
	float currHopTime;

	Sprite * llama;

public:
	Llama(Shader * _shader);
	~Llama();

	void hop();

	void setPath(glm::vec2 _startPos, glm::vec2 _targetPos);
	void addTarget(glm::vec2 _target);
	glm::vec2 getPointOnPath(float _distance);

	virtual void update(Step * _step) override;

};
#pragma once

#include <Entity.h>

class Sprite;
class Shader;

class Llama : public Entity{
private:
	glm::vec2 pathP1;
	glm::vec2 pathP2;

	glm::vec2 hopStartPos;
	glm::vec2 hopEndPos;


	bool isHopping;
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
	glm::vec2 getPointOnPath(float _distance);

	virtual void update(Step * _step) override;

};
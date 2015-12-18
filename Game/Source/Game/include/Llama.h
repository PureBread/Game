#pragma once

#include <Entity.h>

class Sprite;
class Shader;
class Transform;
class SpeechBubble;

class Llama : public Entity{
private:
	glm::vec2 pathP1;
	glm::vec2 pathP2;


	float deltaX;
	float deltaY;

	float angle1;
	float angle2;

protected:
	

public:
Sprite * llama;
	Llama * leader;
	Transform * center;
	SpeechBubble * speechBubble;

	bool isHopping;
	float hopSpeed;
	float hopSpeedMultiplier;
	float hopDuration;
	float hopHeight;
	float hopHeightMultiplier;
	float currHopTime;

	float standDuration;

	float offset;

	std::vector<glm::vec2> targets;
	Llama(Shader * _shader);
	~Llama();

	void hop();

	void setPath(glm::vec2 _startPos, glm::vec2 _targetPos);
	void addTarget(glm::vec2 _target);
	glm::vec2 getPointOnPath(float _distance);

	virtual void update(Step * _step) override;

};
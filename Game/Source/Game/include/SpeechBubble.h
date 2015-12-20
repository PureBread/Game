#pragma once

#include <Sprite.h>

class Camera;
class Llama;

#define NUM_BUBBLES 28

class SpeechBubble : public Sprite{
public:
	float length;
	float duration;

	Camera * cam;
	Llama * llama;

	SpeechBubble(Shader * _shader, Camera * _cam, Llama * _llama);
	~SpeechBubble();

	virtual void update(Step * _step) override;

	void enable();

	void setPos(glm::vec3 _pos);
	glm::vec3 getDistortedScreenPosition(Camera * _cam, glm::vec3 _pos);
};
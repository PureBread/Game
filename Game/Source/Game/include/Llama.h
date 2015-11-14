#pragma once

#include <Sprite.h>

class Llama : public Sprite{
private:
	bool isHopping;

public:
	Llama(Shader * _shader);
	~Llama();

	void hop(glm::vec2 _targetPos);

	virtual void update(Step * _step) override;

};
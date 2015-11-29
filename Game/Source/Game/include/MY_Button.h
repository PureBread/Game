#pragma once

#include <NodeUI.h>
class TextLabel;
class Shader;
class Font;

class MY_Button : public NodeUI{
public:
	TextLabel * label;
	
	Texture * texNormal;
	Texture * texOver;
	Texture * texDown;

	MY_Button(BulletWorld * _world, Font * _font, Shader * _textShader, unsigned long int _size);
	
	void update(Step * _step) override;
};
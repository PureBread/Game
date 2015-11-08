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

	MY_Button(BulletWorld * _world, Scene * _scene, Font * _font, Shader * _textShader, float _width, float _height);
	
	void update(Step * _step) override;
};
#pragma once

#include <NodeUI.h>
class TextLabel;
class Shader;
class Font;
class ComponentShaderText;

class MY_Button : public NodeUI{
public:
	static ComponentShaderText * btnTextShader;
	TextLabel * label;
	
	Texture * texNormal;
	Texture * texOver;
	Texture * texDown;

	MY_Button(BulletWorld * _world, Font * _font, unsigned long int _size);
	
	void update(Step * _step) override;
};
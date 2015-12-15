#pragma once

#include <NodeUI.h>
class TextLabel;
class Shader;
class Font;
class ComponentShaderText;

class MY_Button : public NodeUI{
private:
	TextLabel * label;
	static ComponentShaderText * btnTextShader;
	Texture * texNormal;
	Texture * texOver;
	Texture * texDown;
	std::wstring textToSet;
public:
	
	void setLabel(std::string _text);
	void setLabel(std::wstring _text);

	MY_Button(BulletWorld * _world, Font * _font, unsigned long int _size);
	
	void update(Step * _step) override;
};
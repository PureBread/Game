#pragma once

#include <sweet/UI.h>
#include <PlayerManager.h>
#include <scenario/Conversation.h>
#include <MY_Button.h>

class UI_Controls : public NodeUI{
public:
	/*NodeUI * image;
	TextArea * text;
	MY_Button * optionOne;
	MY_Button * optionTwo;
	MY_Button * nextButton;
	HorizontalLinearLayout * buttonsLayout;*/

	UI_Controls(PlayerManager * _manager, BulletWorld * _world, Shader * _textShader);
	~UI_Controls();
};
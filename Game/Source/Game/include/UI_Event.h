#pragma once

#include <sweet/UI.h>
#include <PlayerManager.h>
#include <scenario/Conversation.h>
#include <MY_Button.h>

class UI_Event : public VerticalLinearLayout, public ConversationIterator{
public:
	NodeUI * image;
	TextArea * text;
	MY_Button * optionOne;
	MY_Button * optionTwo;
	NodeUI * nextButton;

	UI_Event(BulletWorld * _world, Event * _event, Shader * _textShader);
	~UI_Event();
};
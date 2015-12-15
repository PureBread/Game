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
	MY_Button * nextButton;
	HorizontalLinearLayout * buttonsLayout;

	sweet::EventManager * eventManager;

	UI_Event(BulletWorld * _world, Shader * _textShader);
	~UI_Event();

	Event * currentEvent;
	void startEvent(Event * _event);

	virtual bool sayNext() override;

	virtual void update(Step * _step) override;
};
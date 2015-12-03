#pragma once

#include <sweet/UI.h>
#include <PlayerManager.h>
#include <scenario/Conversation.h>
#include <MY_Button.h>

class UI_Controls : public VerticalLinearLayout{
public:
	float slideDuration;
	float currSlideTime;
	bool slideUp;

	/*NodeUI * image;
	TextArea * text;
	MY_Button * optionOne;
	MY_Button * optionTwo;
	MY_Button * nextButton;
	HorizontalLinearLayout * buttonsLayout;*/
	NodeUI * uiHit;
	NodeUI * ui;

	TextLabel * herdSize;

	//NodeUI * dialR1, * dialR2, * dialR3, * dialL1, * dialL2, * dialL3;

	std::vector<NodeUI *> btns;

	UI_Controls(PlayerManager * _manager, BulletWorld * _world, Shader * _textShader);
	~UI_Controls();

	virtual void update(Step * _step) override;

	void disable();
	void enable();

	void setSlide(bool _slideUp);
};
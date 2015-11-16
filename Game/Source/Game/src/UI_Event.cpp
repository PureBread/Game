#pragma once

#include <UI_Event.h>
#include <MY_ResourceManager.h>

UI_Event::UI_Event(BulletWorld * _world, Event * _event, Shader * _textShader) :
	VerticalLinearLayout(_world),
	image(new NodeUI(_world)),
	text(new TextArea(_world, MY_ResourceManager::scenario->getFont("HURLY-BURLY")->font, _textShader, 1.f)),
	nextButton(new NodeUI(_world, kENTITIES, true)),
	optionOne(new MY_Button(_world, MY_ResourceManager::scenario->getFont("HURLY-BURLY")->font, _textShader, 202, 45)),
	optionTwo(new MY_Button(_world, MY_ResourceManager::scenario->getFont("HURLY-BURLY")->font, _textShader, 202, 45))
{
	addChild(image);
	addChild(text);
	addChild(optionOne);
	addChild(optionTwo);

	optionOne->setVisible(false);
	optionTwo->setVisible(false);

	addChild(nextButton);


	image->setWidth(0.9f);
	image->setHeight(0.3f);

	text->setWidth(1.f);
	text->setPadding(32, 0);
	text->setHeight(0.4f);

	image->background->mesh->pushTexture2D(MY_ResourceManager::scenario->defaultTexture->texture);
	nextButton->setBackgroundColour(1,0,0);
}

UI_Event::~UI_Event(){}

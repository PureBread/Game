#pragma once

#include <UI_Controls.h>
#include <MY_ResourceManager.h>

UI_Controls::UI_Controls(PlayerManager * _manager, BulletWorld * _world, Shader * _textShader) :
	NodeUI(_world)
{
	//setRenderMode(kTEXTURE);

	//optionOne->renderMode = kTEXTURE;
	
	//verticalAlignment = kBOTTOM;
	//horizontalAlignment = kCENTER;

	background->setVisible(false);





	SliderControlled * food = new SliderControlled(_world, &_manager->statistics["food"], 0, 100, false);
	SliderControlled * wool = new SliderControlled(_world, &_manager->statistics["wool"], 0, 100, false);
	SliderControlled * health = new SliderControlled(_world, &_manager->statistics["health"], 0, 100);
	SliderController * speed = new SliderController(_world, &_manager->statistics["speed"], 1, 0, 2, false);
	SliderController * rations = new SliderController(_world, &_manager->statistics["rations"], 1, 0, 2, false);
	herdSize = new TextLabel(_world, MY_ResourceManager::scenario->getFont("HURLY-BURLY")->font, _textShader);

	food->fill->background->mesh->pushTexture2D(MY_ResourceManager::scenario->getTexture("SADDLEBAG-R-MASK")->texture);
	wool->fill->background->mesh->pushTexture2D(MY_ResourceManager::scenario->getTexture("SADDLEBAG-L-MASK")->texture);

	herdSize->setText(L"99");
	herdSize->horizontalAlignment = kCENTER;

	
	speed->setStepped(1);
	rations->setStepped(1);
	
	wool->setBackgroundColour(1,1,1);
	wool->fill->setBackgroundColour(1,1,1);
	food->setBackgroundColour(1,1,1);
	food->fill->setBackgroundColour(215.f/255.f, 198.f/255.f, 151.f/255.f);
	health->setBackgroundColour(1,1,1);
	health->fill->setBackgroundColour(129.f/255.f, 208.f/255.f, 217.f/255.f);

	speed->setBackgroundColour(178.f/255.f, 178.f/255.f, 178.f/255.f);
	rations->setBackgroundColour(178.f/255.f, 178.f/255.f, 178.f/255.f);
	
	speed->fill->background->mesh->pushTexture2D(MY_ResourceManager::scenario->getTexture("DIAL-L")->texture);
	rations->fill->background->mesh->pushTexture2D(MY_ResourceManager::scenario->getTexture("DIAL-R")->texture);
	
	wool->background->mesh->pushTexture2D(MY_ResourceManager::scenario->getTexture("SADDLEBAG-L")->texture);
	food->background->mesh->pushTexture2D(MY_ResourceManager::scenario->getTexture("SADDLEBAG-R")->texture);
	

	food->setRationalWidth(0.33f);
	wool->setRationalWidth(0.33f);
	
	food->setMarginTop(0.1f);
	wool->setMarginTop(0.1f);
	
	speed->setRationalWidth(0.1f);
	rations->setRationalWidth(0.1f);
	
	speed->setMarginBottom(0.1f);
	rations->setMarginBottom(0.1f);
	
	speed->setMarginTop(0.2f);
	rations->setMarginTop(0.2f);
	
	health->setMargin(0.25f, 0);
	health->setRationalHeight(0.1f);
	health->setRationalWidth(1.f);

	herdSize->setMargin(0.4f, 0.4f);
	herdSize->setRationalHeight(1.f);
	herdSize->setRationalWidth(1.f);
	

	HorizontalLinearLayout * hlayout = new HorizontalLinearLayout(_world);
	
	hlayout->addChild(wool);
	hlayout->addChild(speed);
	hlayout->addChild(rations);
	hlayout->addChild(food);

	addChild(hlayout);
	addChild(herdSize);
	addChild(health);


	
	hlayout->setRationalWidth(1.f);
	hlayout->setRationalHeight(1.f);
	hlayout->horizontalAlignment = kCENTER;
	hlayout->verticalAlignment = kMIDDLE;
	
	setRationalWidth(1.f);
	setRationalHeight(1.f);
	setMarginTop(0.74);
	setMarginBottom(0.01f);
	setMarginLeft(0.33f);
	setMarginRight(0.33f);
}

UI_Controls::~UI_Controls(){}
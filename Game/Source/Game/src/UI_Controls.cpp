#pragma once

#include <UI_Controls.h>
#include <MY_ResourceManager.h>
#include <Easing.h>

UI_Controls::UI_Controls(PlayerManager * _manager, BulletWorld * _world, Shader * _textShader) :
	VerticalLinearLayout(_world),
	ui(new NodeUI(_world)),
	slideDuration(1.f),
	currSlideTime(0.f),
	slideUp(false)
{

	setRationalHeight(1.f);
	setRationalHeight(1.f);

	eventManager.addEventListener("mousein", [this](sweet::Event * _event){
		test();
	});

	eventManager.addEventListener("mouseout", [this](sweet::Event * _event){
		test();
	});

	//NodeUI * ui = new NodeUI(_world);
	ui->setRationalWidth(1.f);
	ui->setRationalHeight(1.f);

	ui->background->mesh->pushTexture2D(MY_ResourceManager::scenario->getTexture("UI_BACK")->texture);


	SliderControlled * food = new SliderControlled(_world, &_manager->statistics["food"], 0, 100, false);
	SliderControlled * wool = new SliderControlled(_world, &_manager->statistics["wool"], 0, 100, false);
	SliderControlled * health = new SliderControlled(_world, &_manager->statistics["health"], 0, 100);
	herdSize = new TextLabel(_world, MY_ResourceManager::scenario->getFont("HURLY-BURLY")->font, _textShader);
	
	NodeUI * dialR1 = new NodeUI(_world);
	NodeUI * dialR2 = new NodeUI(_world);
	NodeUI * dialR3 = new NodeUI(_world);
	NodeUI * dialL1 = new NodeUI(_world);
	NodeUI * dialL2 = new NodeUI(_world);
	NodeUI * dialL3 = new NodeUI(_world);
	
	dialR1->background->mesh->pushTexture2D(MY_ResourceManager::scenario->getTexture("UI_DIAL-R-1")->texture);
	dialR2->background->mesh->pushTexture2D(MY_ResourceManager::scenario->getTexture("UI_DIAL-R-2")->texture);
	dialR3->background->mesh->pushTexture2D(MY_ResourceManager::scenario->getTexture("UI_DIAL-R-3")->texture);
	dialL1->background->mesh->pushTexture2D(MY_ResourceManager::scenario->getTexture("UI_DIAL-L-1")->texture);
	dialL2->background->mesh->pushTexture2D(MY_ResourceManager::scenario->getTexture("UI_DIAL-L-2")->texture);
	dialL3->background->mesh->pushTexture2D(MY_ResourceManager::scenario->getTexture("UI_DIAL-L-3")->texture);
	
	dialR1->setRationalHeight(1.f);
	dialR2->setRationalHeight(1.f);
	dialR3->setRationalHeight(1.f);
	dialL1->setRationalHeight(1.f);
	dialL2->setRationalHeight(1.f);
	dialL3->setRationalHeight(1.f);
	dialR1->setRationalWidth(1.f);
	dialR2->setRationalWidth(1.f);
	dialR3->setRationalWidth(1.f);
	dialL1->setRationalWidth(1.f);
	dialL2->setRationalWidth(1.f);
	dialL3->setRationalWidth(1.f);

	food->setRationalHeight(1.f);
	food->setRationalWidth(1.f);
	wool->setRationalHeight(1.f);
	wool->setRationalWidth(1.f);
	health->setRationalHeight(1.f);
	health->setRationalWidth(1.f);
	
	food->setMarginTop(0.793f);
	wool->setMarginTop(0.793f);
	health->setMarginTop(0.961f);
	health->setMarginLeft(0.425f);
	health->setMarginRight(1.f - 0.594f);
	
	// setup the fills
	food->fill->background->mesh->pushTexture2D(MY_ResourceManager::scenario->getTexture("UI_FOOD")->texture);
	wool->fill->background->mesh->pushTexture2D(MY_ResourceManager::scenario->getTexture("UI_WOOL")->texture);
	health->fill->background->mesh->pushTexture2D(MY_ResourceManager::scenario->getTexture("UI_HEALTH")->texture);
	wool->fill->setBackgroundColour(1,1,1);
	food->fill->setBackgroundColour(1,1,1);
	health->fill->setBackgroundColour(1,1,1);

	// hide the thumbs and the backgrounds
	wool->thumb->setVisible(false);
	food->thumb->setVisible(false);
	health->thumb->setVisible(false);
	wool->background->setVisible(false);
	food->background->setVisible(false);
	health->background->setVisible(false);
	
	
	herdSize->setText(L"99");
	
	herdSize->setMarginBottom(0.10f);
	herdSize->setRationalWidth(1.f);
	herdSize->horizontalAlignment = kCENTER;
	herdSize->verticalAlignment = kMIDDLE;

	ui->addChild(food);
	ui->addChild(wool);
	ui->addChild(health);
	
	ui->addChild(dialR1);
	ui->addChild(dialR2);
	ui->addChild(dialR3);
	ui->addChild(dialL1);
	ui->addChild(dialL2);
	ui->addChild(dialL3);
	
	ui->addChild(herdSize);

	
	NodeUI * dialR1_btn = new NodeUI(_world, kENTITIES, true);
	NodeUI * dialR2_btn = new NodeUI(_world, kENTITIES, true);
	NodeUI * dialR3_btn = new NodeUI(_world, kENTITIES, true);
	NodeUI * dialL1_btn = new NodeUI(_world, kENTITIES, true);
	NodeUI * dialL2_btn = new NodeUI(_world, kENTITIES, true);
	NodeUI * dialL3_btn = new NodeUI(_world, kENTITIES, true);
	
	dialR1_btn->background->setVisible(false);
	dialR2_btn->background->setVisible(false);
	dialR3_btn->background->setVisible(false);
	dialL1_btn->background->setVisible(false);
	dialL2_btn->background->setVisible(false);
	dialL3_btn->background->setVisible(false);

	dialR1_btn->setRationalHeight(1.f);
	dialR2_btn->setRationalHeight(1.f);
	dialR3_btn->setRationalHeight(1.f);
	dialL1_btn->setRationalHeight(1.f);
	dialL2_btn->setRationalHeight(1.f);
	dialL3_btn->setRationalHeight(1.f);
	dialR1_btn->setRationalWidth(1.f);
	dialR2_btn->setRationalWidth(1.f);
	dialR3_btn->setRationalWidth(1.f);
	dialL1_btn->setRationalWidth(1.f);
	dialL2_btn->setRationalWidth(1.f);
	dialL3_btn->setRationalWidth(1.f);

	dialR1_btn->setMargin(0.50f,0.45f,0.05f,0.90f);
	dialR2_btn->setMargin(0.50f,0.45f,0.10f,0.85f);
	dialR3_btn->setMargin(0.50f,0.45f,0.15f,0.80f);
	dialL1_btn->setMargin(0.45f,0.50f,0.05f,0.90f);
	dialL2_btn->setMargin(0.45f,0.50f,0.10f,0.85f);
	dialL3_btn->setMargin(0.45f,0.50f,0.15f,0.80f);
	

	dialR1_btn->eventManager.addEventListener("click", [this, _manager, dialR1, dialR2, dialR3](sweet::Event * _event){
		dialR1->setVisible(true);
		dialR2->setVisible(false);
		dialR3->setVisible(false);

		_manager->statistics["rations"] = 0;
	});
	dialR2_btn->eventManager.addEventListener("click", [this, _manager, dialR1, dialR2, dialR3](sweet::Event * _event){
		dialR1->setVisible(false);
		dialR2->setVisible(true);
		dialR3->setVisible(false);

		_manager->statistics["rations"] = 1;
	});
	dialR3_btn->eventManager.addEventListener("click", [this, _manager, dialR1, dialR2, dialR3](sweet::Event * _event){
		dialR1->setVisible(false);
		dialR2->setVisible(false);
		dialR3->setVisible(true);

		_manager->statistics["rations"] = 2;
	});
	
	dialL1_btn->eventManager.addEventListener("click", [this, _manager, dialL1, dialL2, dialL3](sweet::Event * _event){
		dialL1->setVisible(true);
		dialL2->setVisible(false);
		dialL3->setVisible(false);

		_manager->statistics["speed"] = 0;
	});
	dialL2_btn->eventManager.addEventListener("click", [this, _manager, dialL1, dialL2, dialL3](sweet::Event * _event){
		dialL1->setVisible(false);
		dialL2->setVisible(true);
		dialL3->setVisible(false);

		_manager->statistics["speed"] = 1;
	});
	dialL3_btn->eventManager.addEventListener("click", [this, _manager, dialL1, dialL2, dialL3](sweet::Event * _event){
		dialL1->setVisible(false);
		dialL2->setVisible(false);
		dialL3->setVisible(true);

		_manager->statistics["speed"] = 2;
	});

	
	ui->addChild(dialR1_btn);
	ui->addChild(dialR2_btn);
	ui->addChild(dialR3_btn);
	ui->addChild(dialL1_btn);
	ui->addChild(dialL2_btn);
	ui->addChild(dialL3_btn);

	addChild(ui);

	invalidateLayout();
}

UI_Controls::~UI_Controls(){}

void UI_Controls::update(Step * _step){
	std::cout << "isHovered: " << isHovered << " slideUp: " << slideUp << "slideDuration: " << slideDuration << " cSlideTime: " << currSlideTime << std::endl;
	/*
	if ((isHovered && !slideUp) || (!isHovered && slideUp)){
		slideUp = !slideUp;
		currSlideTime = std::min(0.f, slideDuration - currSlideTime);
	}*/

	/*
	if (slideUp && currSlideTime <= slideDuration){
		float posY = Easing::easeInOutCubic(currSlideTime, -ui->height.getSize(), ui->height.getSize(), slideDuration);
		ui->childTransform->translate(glm::vec3(0, posY, 0), false);
		currSlideTime += _step->deltaTime;
	} else if(!slideUp && currSlideTime <= slideDuration){
		float posY = Easing::easeInOutCubic(currSlideTime, 0, -ui->height.getSize(), slideDuration);
		ui->childTransform->translate(glm::vec3(0, posY, 0), false);
		currSlideTime += _step->deltaTime;
	}
	*/
	VerticalLinearLayout::update(_step);
}

void UI_Controls::test(){
	slideUp = !slideUp;
	currSlideTime = std::min(0.f, slideDuration - currSlideTime);
}

void UI_Controls::disable(){
	for(NodeUI * n : btns){
		n->setMouseEnabled(false);
	}
}

void UI_Controls::enable(){
	for(NodeUI * n : btns){
		n->setMouseEnabled(true);
	}
}
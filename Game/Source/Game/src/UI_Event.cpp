#pragma once

#include <UI_Event.h>
#include <MY_ResourceManager.h>
#include <scenario/Dialogue.h>

UI_Event::UI_Event(BulletWorld * _world, Shader * _textShader) :
	VerticalLinearLayout(_world),
	image(new NodeUI(_world)),
	text(new TextArea(_world, MY_ResourceManager::scenario->getFont("HURLY-BURLY")->font, _textShader, 1.f)),
	nextButton(new MY_Button(_world, MY_ResourceManager::scenario->getFont("HURLY-BURLY")->font, 1)),
	optionOne(new MY_Button(_world, MY_ResourceManager::scenario->getFont("HURLY-BURLY")->font, 3)),
	optionTwo(new MY_Button(_world, MY_ResourceManager::scenario->getFont("HURLY-BURLY")->font, 3)),
	buttonsLayout(new HorizontalLinearLayout(_world)),
	currentEvent(nullptr),
	eventManager(new sweet::EventManager())
{
	setRenderMode(kTEXTURE);

	//optionOne->renderMode = kTEXTURE;
	
	verticalAlignment = kTOP;
	horizontalAlignment = kCENTER;

	setBackgroundColour(1.f, 1.f, 1.f, 1.f);
	background->mesh->pushTexture2D(MY_ResourceManager::scenario->getTexture("SCROLL_EVENT")->texture);

	
	setMargin(0.05f);
	setPadding(0.125f, 0.125f);
	setRationalWidth(1.f);
	setRationalHeight(1.f);

	addChild(image);
	addChild(text);
	addChild(buttonsLayout);
	buttonsLayout->addChild(optionOne);
	buttonsLayout->addChild(nextButton);
	buttonsLayout->addChild(optionTwo);

	image->setRationalWidth(1.0f);
	image->setRationalHeight(0.8f);
	image->background->mesh->scaleModeMag = image->background->mesh->scaleModeMin = GL_NEAREST;

	text->setWidth(1.f);
	text->setHeight(MY_ResourceManager::scenario->getFont("HURLY-BURLY")->font->getLineHeight()*2);
	//text->setHeight(0.4f);
	text->setText(L"Bacon ipsum dolor amet anim occaecat pork loin.");
	text->verticalAlignment = kTOP;
	text->setWrapMode(kWORD);

	nextButton->setLabel("NEXT");

	nextButton->eventManager.addEventListener("click", [this](sweet::Event * _event){
		eventManager->triggerEvent("next");
	});
	optionOne->eventManager.addEventListener("click", [this](sweet::Event * _event){
		sweet::Event * e = new sweet::Event("select");
		e->setIntData("option", 0);
		eventManager->triggerEvent(e);
	});
	optionTwo->eventManager.addEventListener("click", [this](sweet::Event * _event){
		sweet::Event * e = new sweet::Event("select");
		e->setIntData("option", 1);
		eventManager->triggerEvent(e);
	});

	
	// prevent the user from clicking buttons while they're not visible
	nextButton->setMouseEnabled(false);
	optionOne->setMouseEnabled(false);
	optionTwo->setMouseEnabled(false);
	setVisible(false);


	eventManager->addEventListener("select", [this](sweet::Event * _event){
		select(_event->getIntData("option"));
	});eventManager->addEventListener("next", [this](sweet::Event * _event){
		if(!this->sayNext()){
			// prevent the user from clicking buttons while they're not visible
			nextButton->setMouseEnabled(false);
			optionOne->setMouseEnabled(false);
			optionTwo->setMouseEnabled(false);
			setVisible(false);
		}
	});
}

UI_Event::~UI_Event(){
	delete eventManager;
}

void UI_Event::update(Step * _step){
	eventManager->update(_step);
	VerticalLinearLayout::update(_step);
}

void UI_Event::startEvent(Event * _event){
	MY_ResourceManager::scenario->getAudio("EVENT_OPEN")->sound->play();
	currentEvent = _event;
	setVisible(true);

	currentConversation = _event->scenario->conversations["intro"];
	currentConversation->reset();
	sayNext();
}

bool UI_Event::sayNext(){
	invalidateLayout();
	if(ConversationIterator::sayNext()){
		// remove existing images
		while(image->background->mesh->textures.size() > 0){
			delete image->background->mesh->textures.back();
			image->background->mesh->textures.pop_back();
		}
		// make the image "textures/event/SCENARIO_CONVERSATION.png"
		std::stringstream imgSrc;
		imgSrc << currentEvent->scenario->id << "_" << currentConversation->id << ".png";
		Texture * tex = new Texture(imgSrc.str(), false, false, false);
		tex->load();
		image->background->mesh->pushTexture2D(tex);

		text->setText(currentConversation->getCurrentDialogue()->getCurrentText());

		if(waitingForInput){
			nextButton->setVisible(false);
			optionOne->setVisible(true);
			optionTwo->setVisible(true);
			
			nextButton->setMouseEnabled(false);
			optionOne->setMouseEnabled(true);
			optionTwo->setMouseEnabled(true);
			
			optionOne->setLabel(currentConversation->options.at(0)->text);
			optionTwo->setLabel(currentConversation->options.at(1)->text);
		}else{
			nextButton->setVisible(true);
			optionOne->setVisible(false);
			optionTwo->setVisible(false);
			
			nextButton->setMouseEnabled(true);
			optionOne->setMouseEnabled(false);
			optionTwo->setMouseEnabled(false);
		}

		return true;
	}else{
		return false;
	}
}
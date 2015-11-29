#pragma once

#include <UI_Event.h>
#include <MY_ResourceManager.h>
#include <scenario/Dialogue.h>

UI_Event::UI_Event(BulletWorld * _world, Shader * _textShader) :
	VerticalLinearLayout(_world),
	image(new NodeUI(_world)),
	text(new TextArea(_world, MY_ResourceManager::scenario->getFont("HURLY-BURLY")->font, _textShader, 1.f)),
	nextButton(new NodeUI(_world, kENTITIES, true)),
	optionOne(new MY_Button(_world, MY_ResourceManager::scenario->getFont("HURLY-BURLY")->font, _textShader, 202, 45)),
	optionTwo(new MY_Button(_world, MY_ResourceManager::scenario->getFont("HURLY-BURLY")->font, _textShader, 202, 45)),
	done(false),
	currentEvent(nullptr)
{
	setRenderMode(kTEXTURE);

	//optionOne->renderMode = kTEXTURE;
	setBackgroundColour(1.f, 1.f, 1.f, 1.f);
	background->mesh->pushTexture2D(MY_ResourceManager::scenario->getTexture("SCROLL_EVENT")->texture);

	addChild(image);
	addChild(text);
	addChild(optionOne);
	addChild(optionTwo);

	optionOne->setVisible(false);
	optionTwo->setVisible(false);

	addChild(nextButton);


	image->setWidth(1.0f);
	image->setHeight(0.3f);
	image->background->mesh->scaleModeMag = image->background->mesh->scaleModeMin = GL_NEAREST;

	text->setWidth(1.f);
	text->setPadding(32, 0);
	text->setHeight(0.4f);
	text->setText(L"Bacon ipsum dolor amet anim occaecat pork loin.");

	nextButton->setBackgroundColour(1,1,0);
	nextButton->setWidth(100);
	nextButton->setHeight(100);

	nextButton->eventManager.addEventListener("click", [this](sweet::Event * _event){
		done = !this->sayNext();
	});
	optionOne->eventManager.addEventListener("click", [this](sweet::Event * _event){
		select(0);
	});
	optionTwo->eventManager.addEventListener("click", [this](sweet::Event * _event){
		select(1);
	});

	setVisible(false);
}

UI_Event::~UI_Event(){}


void UI_Event::startEvent(Event * _event){
	currentEvent = _event;
	done = false;
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
			
			optionOne->label->setText(currentConversation->options.at(0)->text);
			optionTwo->label->setText(currentConversation->options.at(1)->text);
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
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
	addChild(image);
	addChild(text);
	addChild(optionOne);
	addChild(optionTwo);

	optionOne->setVisible(false);
	optionTwo->setVisible(false);

	addChild(nextButton);


	image->setWidth(0.9f);
	image->setHeight(0.3f);
	image->background->mesh->scaleModeMag = image->background->mesh->scaleModeMin = GL_NEAREST;

	text->setWidth(1.f);
	text->setPadding(32, 0);
	text->setHeight(0.4f);
	text->setText(L"Bacon ipsum dolor amet anim occaecat pork loin.");

	nextButton->setBackgroundColour(1,1,0);
	nextButton->setWidth(100);
	nextButton->setHeight(100);

	nextButton->onClickFunction = [this](){
		done = !this->sayNext();
	};

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

		std::string t = currentConversation->getCurrentDialogue()->getCurrentText();
		text->setText(std::wstring(t.begin(), t.end()));
		return true;
	}else{
		return false;
	}
}
#pragma once

#include <MY_Button.h>

#include <sweet/UI.h>
#include <MY_ResourceManager.h>
#include <MeshFactory.h>
#include <shader/ComponentShaderBase.h>
#include <shader/ComponentShaderText.h>
#include <NumberUtils.h>

ComponentShaderText * MY_Button::btnTextShader = nullptr;

MY_Button::MY_Button(BulletWorld * _world, Font * _font, unsigned long int _size) :
	NodeUI(_world, kENTITIES, true),
	label(nullptr),
	textToSet(L"")
{
	if (btnTextShader == nullptr){
		btnTextShader = new ComponentShaderText(true);
		btnTextShader->setColor(1.f, 1.f, 1.f);
	}

	label = new TextLabel(_world, _font, btnTextShader);

	if(_size == 1){
		setWidth(202);
	}else if(_size == 2){
		setWidth(242);
	}else{
		setWidth(282);
	}
	setHeight(45);
	setMargin(0, 5);

	std::stringstream ss;
	ss << "BUTTON" << _size;
	texNormal = MY_ResourceManager::scenario->getTexture(ss.str() + "_NORMAL")->texture;
	texOver = MY_ResourceManager::scenario->getTexture(ss.str() + "_OVER")->texture;
	texDown = MY_ResourceManager::scenario->getTexture(ss.str() + "_DOWN")->texture;

	background->mesh->pushTexture2D(texNormal);

	label->horizontalAlignment = kCENTER;
	label->setRationalWidth(1.f);

	VerticalLinearLayout * vl = new VerticalLinearLayout(world);
	vl->addChild(label);
	//vl->horizontalAlignment = kCENTER;
	vl->verticalAlignment = kTOP;
	vl->setRationalWidth(1.f);
	vl->setRationalHeight(1.f);
	addChild(vl);

	eventManager.addEventListener("click", [](sweet::Event * _event){
		std::stringstream ss;
		ss << "BUTTON" << sweet::NumberUtils::randomInt(1,3);
		MY_ResourceManager::scenario->getAudio(ss.str())->sound->play();
	});
}

void MY_Button::update(Step * _step){
	Texture * texNow = background->mesh->getTexture(0);
	Texture * texNew;
	if(isHovered){
		if(isDown){
			texNew = texDown;
		}else{
			texNew = texOver;
		}
	}else{
		texNew = texNormal;
	}
	if(texNow != texNew){
		invalidateRenderFrame();
		background->mesh->removeTextureAt(0);
		background->mesh->pushTexture2D(texNew);
	}

	if(textToSet != L""){
		label->setText(textToSet);
		textToSet = L"";
	}
	NodeUI::update(_step);
}

void MY_Button::setLabel(std::string _text){
	textToSet = std::wstring(_text.begin(), _text.end());
}
void MY_Button::setLabel(std::wstring _text){
	textToSet = _text;
}
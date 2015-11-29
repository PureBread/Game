#pragma once

#include <MY_Button.h>

#include <sweet/UI.h>
#include <MY_ResourceManager.h>
#include <MeshFactory.h>
#include <shader/ComponentShaderBase.h>
#include <NumberUtils.h>

MY_Button::MY_Button(BulletWorld * _world, Font * _font, Shader * _textShader, float _width, float _height) :
	NodeUI(_world, kENTITIES, true),
	label(new TextLabel(_world, _font, _textShader))
{
	setWidth(_width);
	setHeight(_height);
	setMargin(0, 5);

	texNormal = MY_ResourceManager::scenario->getTexture("BUTTON_NORMAL")->texture;
	texOver = MY_ResourceManager::scenario->getTexture("BUTTON_OVER")->texture;
	texDown = MY_ResourceManager::scenario->getTexture("BUTTON_DOWN")->texture;

	background->mesh->pushTexture2D(texNormal);

	label->horizontalAlignment = kCENTER;

	VerticalLinearLayout * vl = new VerticalLinearLayout(world);
	vl->addChild(label);
	vl->horizontalAlignment = kCENTER;
	vl->verticalAlignment = kTOP;
	vl->setWidth(1.f);
	vl->setHeight(1.f);
	addChild(vl);

	eventManager.addEventListener("click", [](sweet::Event * _event){
		std::stringstream ss;
		ss << "BUTTON" << sweet::NumberUtils::randomInt(1,5);
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
	NodeUI::update(_step);
}
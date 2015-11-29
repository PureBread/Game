#pragma once

#include <MY_Button.h>

#include <sweet/UI.h>
#include <MY_ResourceManager.h>
#include <MeshFactory.h>
#include <shader/ComponentShaderBase.h>
#include <NumberUtils.h>

MY_Button::MY_Button(BulletWorld * _world, Font * _font, Shader * _textShader, unsigned long int _size) :
	NodeUI(_world, kENTITIES, true),
	label(new TextLabel(_world, _font, _textShader))
{
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

	VerticalLinearLayout * vl = new VerticalLinearLayout(world);
	vl->addChild(label);
	vl->horizontalAlignment = kCENTER;
	vl->verticalAlignment = kTOP;
	vl->setRationalWidth(1.f);
	vl->setRationalHeight(1.f);
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
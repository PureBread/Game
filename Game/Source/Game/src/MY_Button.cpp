#pragma once

#include <MY_Button.h>

#include <sweet/UI.h>
#include <MY_ResourceManager.h>
#include <MeshFactory.h>
#include <shader/ComponentShaderBase.h>

MY_Button::MY_Button(BulletWorld * _world, Scene * _scene, Font * _font, Shader * _textShader, float _width, float _height) :
	NodeUI(_world, _scene, kENTITIES, true),
	label(new TextLabel(_world, _scene, _font, _textShader))
{
	setWidth(_width);
	setHeight(_height);
	setMargin(0, 5);

	texNormal = MY_ResourceManager::scenario->getTexture("BUTTON_NORMAL")->texture;
	texOver = MY_ResourceManager::scenario->getTexture("BUTTON_OVER")->texture;
	texDown = MY_ResourceManager::scenario->getTexture("BUTTON_DOWN")->texture;

	background->mesh->pushTexture2D(texNormal);

	VerticalLinearLayout * vl = new VerticalLinearLayout(world, scene);
	vl->addChild(label);
	vl->horizontalAlignment = kCENTER;
	vl->verticalAlignment = kMIDDLE;
	vl->setWidth(_width);
	vl->setHeight(_height);
	addChild(vl);
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
		background->mesh->removeTextureAt(0);
		background->mesh->pushTexture2D(texNew);
	}
	NodeUI::update(_step);
}
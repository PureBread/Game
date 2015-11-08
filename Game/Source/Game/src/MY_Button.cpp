#pragma once

#include <MY_Button.h>

#include <sweet/UI.h>
#include <MY_ResourceManager.h>

MY_Button::MY_Button(BulletWorld * _world, Scene * _scene, Font * _font, Shader * _textShader, float _width, float _height) :
	NodeUI(_world, _scene, kENTITIES, true),
	label(new TextLabel(_world, _scene, _font, _textShader))
{
	setWidth(_width);
	setHeight(_height);
	setMargin(0, 5);

	background->mesh->pushTexture2D(MY_ResourceManager::scenario->getTexture("BUTTON_NORMAL")->texture);

	VerticalLinearLayout * vl = new VerticalLinearLayout(world, scene);
	vl->addChild(label);
	vl->horizontalAlignment = kCENTER;
	vl->verticalAlignment = kMIDDLE;
	vl->setWidth(_width);
	vl->setHeight(_height);
	addChild(vl);
}

void MY_Button::update(Step * _step){
	NodeUI::update(_step);
	if(isHovered){
		if(isDown){
			setBackgroundColour(0, -1, -1);
		}else{
			setBackgroundColour(-1, 0, -1);
		}
	}else{
		setBackgroundColour(-1, -1, 0);
	}
}
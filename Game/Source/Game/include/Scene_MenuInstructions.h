#pragma once

#include <Scene_Menu.h>
class Slider;
class TextLabel;

class Scene_MenuInstructions : public Scene_Menu{
public:
	virtual void update(Step * _step) override;
	

	Scene_MenuInstructions(Game * _game);
	~Scene_MenuInstructions();
};
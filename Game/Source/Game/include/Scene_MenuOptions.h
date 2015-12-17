#pragma once

#include <Scene_Menu.h>
#include <MY_Game.h>
class Slider;
class TextLabel;

class Scene_MenuOptions : public Scene_Menu{
public:
	virtual void update(Step * _step) override;
	
	Slider * volume;
	TextLabel * volumeText;
	float newVolume;

	Scene_MenuOptions(MY_Game * _game);
	~Scene_MenuOptions();
};
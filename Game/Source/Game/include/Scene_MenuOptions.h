#pragma once

#include <Scene_Menu.h>
class Slider;
class TextLabel;

class Scene_MenuOptions : public Scene_Menu{
public:
	virtual void update(Step * _step) override;
	
	Slider * volume;
	TextLabel * volumeText;

	Scene_MenuOptions(Game * _game);
	~Scene_MenuOptions();
};
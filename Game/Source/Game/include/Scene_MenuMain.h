#pragma once

#include <Scene_Menu.h>

class Scene_MenuMain : public Scene_Menu{
public:
	virtual void update(Step * _step) override;

	Scene_MenuMain(MY_Game * _game);
	~Scene_MenuMain();
};
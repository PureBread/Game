#pragma once

#include <MY_Game.h>
#include <MY_Scene.h>
#include <Scene_MenuMain.h>

MY_Game::MY_Game() :
	Game(true, std::pair<std::string, Scene *>("MENU_MAIN", new Scene_MenuMain(this)), false)
{
	scenes["MAIN"] = new MY_Scene(this);
}

MY_Game::~MY_Game(){

}
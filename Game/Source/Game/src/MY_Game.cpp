#pragma once

#include <MY_Game.h>
#include <MY_Scene.h>
#include <Scene_MenuMain.h>
#include <Scene_MenuOptions.h>

MY_Game::MY_Game() :
	Game(true, std::pair<std::string, Scene *>("MENU_MAIN", new Scene_MenuMain(this)), false)
{
	scenes["MAIN"] = new MY_Scene(this);
	scenes["MENU_OPTIONS"] = new Scene_MenuOptions(this);


	MY_ResourceManager::scenario->getAudio("BGM")->sound->play(true);
}

MY_Game::~MY_Game(){

}
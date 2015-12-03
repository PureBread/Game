#pragma once

#include <MY_Game.h>
#include <MY_Scene.h>
#include <Scene_MenuMain.h>
#include <Scene_MenuOptions.h>
#include <Scene_MenuInstructions.h>

MY_Game::MY_Game() :
	Game(true, std::pair<std::string, Scene *>("MENU_MAIN", new Scene_MenuMain(this)), false)
{
	scenes["MAIN"] = new MY_Scene(this);
	scenes["MENU_OPTIONS"] = new Scene_MenuOptions(this);
	scenes["MENU_INSTRUCTIONS"] = new Scene_MenuInstructions(this);

	MY_ResourceManager::scenario->getAudio("ANDES")->sound->play(true);
}

MY_Game::~MY_Game(){

}
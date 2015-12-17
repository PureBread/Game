#pragma once

#include <MY_Game.h>
#include <MY_Scene.h>
#include <Scene_MenuMain.h>
#include <Scene_MenuOptions.h>
#include <Scene_MenuInstructions.h>

bool MY_Game::casualMode = false;

MY_Game::MY_Game() :
	Game(true, std::pair<std::string, Scene *>("MENU_MAIN", new Scene_MenuMain(this)), false)
{

	MY_ResourceManager::scenario->getAudio("ANDES")->sound->play(true);
}

MY_Game::~MY_Game(){

}
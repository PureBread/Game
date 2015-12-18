#pragma once

#include <MY_Game.h>
#include <MY_Scene.h>
#include <MY_ResourceManager.h>
#include <Scene_MenuMain.h>
#include <Scene_MenuOptions.h>
#include <Scene_MenuInstructions.h>
#include <Scene_Splash.h>
#include <Scene_Splash_Engine.h>

bool MY_Game::casualMode = false;
OpenAL_Sound * MY_Game::currentAudio = nullptr;

void MY_Game::addSplashes(){
	Game::addSplashes(); // add engine default splashes
	addSplash(new Scene_Splash(this, new Texture("assets/textures/splash-purebread.png", false, true), Scenario::defaultAudio->sound));
}

MY_Game::MY_Game() :
	Game(std::pair<std::string, Scene *>("MENU_MAIN", new Scene_MenuMain(this)))
{
	scenes["MAIN"] = new MY_Scene(this);
	//MY_ResourceManager::scenario->getAudio("ANDES")->sound->play(true);
}

MY_Game::~MY_Game(){

}

void MY_Game::switchAudio(std::string _newAudio){
	if(currentAudio != nullptr){
		currentAudio->stop();
	}
	currentAudio = MY_ResourceManager::scenario->getAudio(_newAudio)->sound;
	currentAudio->play(true);
}
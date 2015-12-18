#pragma once

#include <Game.h>
class OpenAL_Sound;
class MY_Game : public Game{
protected:
	void addSplashes() override;
public:
	static bool casualMode;
	
	static OpenAL_Sound * currentAudio;

	static void switchAudio(std::string _newAudio);

	MY_Game();
	~MY_Game();
};
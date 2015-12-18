#pragma once

#include <Game.h>

class MY_Game : public Game{
protected:
	void addSplashes() override;
public:
	static bool casualMode;

	MY_Game();
	~MY_Game();
};
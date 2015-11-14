#pragma once

#include <ResourceManager.h>
#include <scenario\Scenario.h>

class MY_ResourceManager : public ResourceManager{
public:
	static std::vector<Scenario *> randomEvents;
	static std::vector<Scenario *> lossEvents;
	static std::vector<Scenario *> destinationEvents;

	static Scenario * scenario;
	static void init();
};
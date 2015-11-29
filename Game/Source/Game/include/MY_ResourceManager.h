#pragma once

#include <ResourceManager.h>
#include <scenario\Scenario.h>
#include <EventManager.h>

class MY_ResourceManager : public ResourceManager{
public:
	static std::vector<Scenario *> randomEvents;
	static std::vector<Scenario *> lossEvents;
	static std::vector<Scenario *> destinationEvents;

	static Scenario * scenario;
	static sweet::EventManager globalEventManager;
	static void init();
};
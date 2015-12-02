#pragma once

#include <node\NodeUpdatable.h>
#include <scenario\Scenario.h>
#include <map>

#include <EventManager.h>
#include <Markers.h>
#include <LevelPath.h>

class ShaderComponentReplace;

enum EventType {
	kDESTINATION = 0,
	kLOSS = 1,
	kRANDOM = 2
};

class Event{
public:
	EventType type;
	Scenario * scenario;

	Event(EventType _type, Scenario * _scenario);
};

class PlayerManager : public NodeUpdatable{
private:

	Event * eventToTrigger;
	float momentDelay;
	float momentTimer;
	std::string destinationEvent;

	bool shouldTriggerDestinationEvent();
	bool shouldTriggerLossEvent();
	bool shouldTriggerRandomEvent();
	Event * triggerDestinationEvent();
	Event * triggerLossEvent();
	Event * triggerRandomEvent();
public:
	// container for all of the event managers on the event scenarios
	sweet::EventManager globalEventManager;
	Markers markers;
	LevelPath * levelPath;
	std::map<std::string, float> statistics;

	// global multipliers applied regardless of player input
	float speedMultiplier, woolMultiplier, rationsMultiplier, healthMultiplier;
	// chance of event occurring in a single moment (0-100)
	float randomEventBaseChance, lossEventBaseChance;


	// saves resources to a json file at data/_path
	void save(const std::string & _path);
	// loads resources from a json file at data/_path
	void load(const std::string & _path);
	// loads the default resources
	void loadDefaults();

	PlayerManager(ShaderComponentReplace * _replaceComponent);
	~PlayerManager();

	virtual void update(Step * _step) override;



	void moment();

	// if an event is available, return it and removes the reference from the manager
	// if no event is available, returns nullptr
	// NOTE: this function does not delete events; it only consumes the reference
	Event * consumeEvent();

	void addLlama(Shader * _shader, bool _isLeader = false);
};
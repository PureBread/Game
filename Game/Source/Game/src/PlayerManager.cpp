#pragma once

#include <PlayerManager.h>
#include <json\json.h>
#include <FileUtils.h>
#include <Log.h>
#include <Step.h>
#include <NumberUtils.h>
#include <MY_ResourceManager.h>

Event::Event(EventType _type, Scenario * _scenario) :
	type(_type),
	scenario(_scenario)
{
}

PlayerManager::PlayerManager() :
	momentDelay(0.5f),
	momentTimer(0),
	speedMultiplier(0.1f),
	eventToTrigger(nullptr),
	levelPath(new LevelPath("walkLayer.png"))
{
	loadDefaults();
	
	markers.eventManager.listeners["destination"].push_back([this](sweet::Event * _e){std::cout << _e->getStringData("scenario");});
}

PlayerManager::~PlayerManager(){
	if(eventToTrigger != nullptr){
		delete eventToTrigger;
	}
}

void PlayerManager::save(const std::string & _path){
	Json::Value jsonStatistics;
	for(auto k : statistics){
		jsonStatistics[k.first] = k.second;
	}	
	Json::Value saveFile;
	saveFile["statistics"] = jsonStatistics;
	std::ofstream log("data/"+_path, std::ios_base::out);
	log << saveFile;
	log.close();
}

void PlayerManager::load(const std::string & _path){
	Json::Value root;
	Json::Reader reader;
	std::string jsonLoaded = FileUtils::readFile("data/" + _path);
	bool parsingSuccessful = reader.parse( jsonLoaded, root );
	if(!parsingSuccessful){
		Log::error("JSON parse failed: " + reader.getFormattedErrorMessages()/* + "\n" + jsonLoaded*/);
	}else{
		 Json::Value resourcesJson = root["statistics"];
		 Json::Value::Members keys = resourcesJson.getMemberNames();
		 for(auto key : keys) {
			 statistics[key] = resourcesJson[key].asFloat();
		 }
	}
}

void PlayerManager::loadDefaults(){
	load("../assets/statisticsDefaults.json");
}

void PlayerManager::update(Step * _step){
	// increase the timer
	momentTimer += _step->deltaTime;
	
	// call moment only when enough time has passed
	while(momentTimer > momentDelay){
		moment();
		momentTimer -= momentDelay;
	}
}

void PlayerManager::moment(){
	// update statistics
	statistics["progress"] += statistics["speed"]*speedMultiplier;
	std::cout << "Progress: " << statistics["progress"] << std::endl;
	statistics["wool"] += statistics["herdSize"];
	if(statistics["food"] <= 0){
		statistics["rations"] = 0;
	}else{
		statistics["food"] -= statistics["rations"] * statistics["herdSize"];
	}
	statistics["health"] += (statistics["rations"]-1)*1.5f - (statistics["speed"] - 1);

	// check for events
	markers.currentPosition = statistics["progress"];
	markers.update(nullptr);

	if(shouldTriggerDestinationEvent()){
		eventToTrigger = triggerDestinationEvent();
	}else if(shouldTriggerLossEvent()){
		eventToTrigger = triggerLossEvent();
	}else if(shouldTriggerRandomEvent()){
		eventToTrigger = triggerRandomEvent();
	}
}


bool PlayerManager::shouldTriggerDestinationEvent(){
	// check progress against markers
	return false;
}

bool PlayerManager::shouldTriggerLossEvent(){
	// random check which is more likely to succeed when health is low
	return sweet::NumberUtils::randomFloat(0, 100) < (5 * (100 - statistics["health"])/100);
}

bool PlayerManager::shouldTriggerRandomEvent(){
	// completely random check
	return sweet::NumberUtils::randomFloat(0, 100) < 5;
}

Event * PlayerManager::triggerDestinationEvent(){
	return new Event(kDESTINATION, MY_ResourceManager::scenario);
}

Event * PlayerManager::triggerLossEvent(){
	bool eventValid = true;
	unsigned long int selectedEvent;
	Scenario * scenario;
	do{
		// get random event
		selectedEvent = sweet::NumberUtils::randomInt(0, MY_ResourceManager::lossEvents.size()-1);
		scenario = MY_ResourceManager::lossEvents.at(selectedEvent);
		// check if event is valid
	}while(!eventValid); // while random event 

	// if the event is non-repeatable, remove it from the active event list
	if(false){
		MY_ResourceManager::lossEvents.erase(MY_ResourceManager::lossEvents.begin()+selectedEvent);
	}
	
	return new Event(kLOSS, scenario);
}

Event * PlayerManager::triggerRandomEvent(){
	bool eventValid = true;
	unsigned long int selectedEvent;
	Scenario * scenario;
	do{
		// get random event
		selectedEvent = sweet::NumberUtils::randomInt(0, MY_ResourceManager::randomEvents.size()-1);
		scenario = MY_ResourceManager::randomEvents.at(selectedEvent);
		// check if event is valid
	}while(!eventValid); // while random event 

	// if the event is non-repeatable, remove it from the active event list
	if(false){
		MY_ResourceManager::randomEvents.erase(MY_ResourceManager::randomEvents.begin()+selectedEvent);
	}

	return new Event(kRANDOM, scenario);
}

Event * PlayerManager::consumeEvent(){
	Event * res = eventToTrigger;
	eventToTrigger = nullptr;
	return res;
}
#pragma once

#include <PlayerManager.h>
#include <LlamaLeader.h>
#include <json\json.h>
#include <FileUtils.h>
#include <Log.h>
#include <Step.h>
#include <NumberUtils.h>
#include <MY_ResourceManager.h>
#include <MY_Game.h>
#include <Sprite.h>

Event::Event(EventType _type, Scenario * _scenario) :
	type(_type),
	scenario(_scenario)
{
}

PlayerManager::PlayerManager(ShaderComponentReplace * _replaceComponent) :
	momentDelay(0.016f),
	momentTimer(0),
	speedMultiplier(0.0015f),
	woolMultiplier(0.0015f),
	rationsMultiplier(0.003f),
	healthMultiplier(0.005f),
	randomEventBaseChance(0.15f),
	lossEventBaseChance(0.3f),
	eventToTrigger(nullptr),
	destinationEvent(""),
	levelPath(new LevelPath("walkLayer.png", _replaceComponent))
{
	loadDefaults();
	
	markers.eventManager.listeners["destination"].push_back([this](sweet::Event * _e){
		destinationEvent = _e->getStringData("scenario");
	});


	// add all of the scenario event managers as children of the global event manager
	for(Scenario * s : MY_ResourceManager::randomEvents){
		globalEventManager.addChildManager(&s->eventManager);
	}for(Scenario * s : MY_ResourceManager::lossEvents){
		globalEventManager.addChildManager(&s->eventManager);
	}for(Scenario * s : MY_ResourceManager::destinationEvents){
		globalEventManager.addChildManager(&s->eventManager);
	}

	// add listeners to global event manager
	globalEventManager.addEventListener("statisticUpdate", [this](sweet::Event * _event){
		std::stringstream ss;
		ss << _event->tag << ": " << _event->getStringData("statistic") << " " << _event->getFloatData("amount");
		Log::info(ss.str());

		statistics[_event->getStringData("statistic")] += _event->getFloatData("amount");
	});

	
	globalEventManager.addEventListener("destinationUpdate", [this](sweet::Event * _event){
		Log::info(_event->tag);

		statistics["food"] = statistics["food"]*2 + statistics["wool"];
		statistics["wool"] = 0;
		statistics["health"] = 100;
	});
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
	
	if(!MY_Game::casualMode){
		statistics["wool"] += statistics["herdSize"]*woolMultiplier;
		float r = statistics["food"] <= FLT_EPSILON ? 0 : statistics["rations"];
		statistics["food"] -= (r * statistics["herdSize"])*rationsMultiplier;
		statistics["health"] += ((r-1)*1.5f - (statistics["speed"] - 1))*healthMultiplier;
	}

	// clamp statistics
	statistics["health"] = std::max(0.f, std::min(statistics["health"], 100.f));
	statistics["wool"] = std::max(0.f, std::min(statistics["wool"], 100.f));
	statistics["food"] = std::max(0.f, std::min(statistics["food"], 100.f));

	// check for events
	markers.currentPosition = statistics["progress"];
	markers.update(nullptr);
	
	if(usedRandomScenarios.size() == MY_ResourceManager::randomEvents.size()){
		usedRandomScenarios.clear();
	}
	if(usedLossScenarios.size() == MY_ResourceManager::lossEvents.size()){
		usedLossScenarios.clear();
	}

	if(shouldTriggerDestinationEvent()){
		eventToTrigger = triggerDestinationEvent();
	}else if(shouldTriggerLossEvent()){
		eventToTrigger = triggerLossEvent();
	}else if(shouldTriggerRandomEvent()){
		eventToTrigger = triggerRandomEvent();
	}

	levelPath->setProgress(statistics["progress"]+0.25f);
}


bool PlayerManager::shouldTriggerDestinationEvent(){
	// check progress against markers
	return destinationEvent != "";
}

bool PlayerManager::shouldTriggerLossEvent(){
	// random check which is more likely to succeed when health is low
	return !MY_Game::casualMode && sweet::NumberUtils::randomFloat(0, 100) < (lossEventBaseChance * (100 - statistics["health"])/100);
}

bool PlayerManager::shouldTriggerRandomEvent(){
	// completely random check
	return sweet::NumberUtils::randomFloat(0, 100) < randomEventBaseChance;
}

Event * PlayerManager::triggerDestinationEvent(){
	std::string d = "assets/events/destination/" + destinationEvent + ".json";
	for(unsigned long int i = 0; i < MY_ResourceManager::destinationEvents.size(); ++i){
		if(MY_ResourceManager::destinationEvents.at(i)->id == d){
			Event * e = new Event(kDESTINATION, MY_ResourceManager::destinationEvents.at(i));
			
			MY_Game::switchAudio(destinationEvent);

			destinationEvent = "";
			return e;
		}
	}
	return nullptr;
}

bool PlayerManager::randomEventIsValid(Scenario * _event){
	for(unsigned long int i = 0; i < usedRandomScenarios.size(); ++i){
		if(usedRandomScenarios.at(i) == _event){
			return false;
		}
	}
	return true;
}

bool PlayerManager::lossEventIsValid(Scenario * _event){
	for(unsigned long int i = 0; i < usedLossScenarios.size(); ++i){
		if(usedLossScenarios.at(i) == _event){
			return false;
		}
	}
	return true;
}

Event * PlayerManager::triggerLossEvent(){
	unsigned long int selectedEvent;
	Scenario * scenario;
	do{
		// get random event
		selectedEvent = sweet::NumberUtils::randomInt(0, MY_ResourceManager::lossEvents.size()-1);
		scenario = MY_ResourceManager::lossEvents.at(selectedEvent);
	}while(!lossEventIsValid(scenario));
	usedLossScenarios.push_back(scenario);
	
	return new Event(kLOSS, scenario);
}

Event * PlayerManager::triggerRandomEvent(){
	unsigned long int selectedEvent;
	Scenario * scenario;
	do{
		// get random event
		selectedEvent = sweet::NumberUtils::randomInt(0, MY_ResourceManager::randomEvents.size()-1);
		scenario = MY_ResourceManager::randomEvents.at(selectedEvent);
	}while(!randomEventIsValid(scenario)); 
	usedRandomScenarios.push_back(scenario);

	return new Event(kRANDOM, scenario);
}

Event * PlayerManager::consumeEvent(){
	Event * res = eventToTrigger;
	eventToTrigger = nullptr;
	return res;
}

void PlayerManager::addLlama(Shader * _shader, bool _isLeader){
	Llama * llama;

	if (_isLeader){
		llama = new LlamaLeader(_shader);
	}else {
		llama = new Llama(_shader);
		llama->leader = levelPath->llamas.front();
		// move llama slightly back
		// we can't have an offset really without going thorugh all the vertices to calculate a posotion, maybe we could?
		// I don't know, place llama at position of leader?
		llama->offset = (float)sweet::NumberUtils::randomInt(1, 50) / 200.f;

		// adjust llama attributes
		llama->hopSpeed = (float)sweet::NumberUtils::randomInt(10, 30) / 100.f;
		llama->hopSpeedMultiplier = (float)sweet::NumberUtils::randomInt(70, 100) / 100.f;

		llama->hopHeightMultiplier = (float)sweet::NumberUtils::randomInt(80, 130) / 100.f;

		// randomize llama's size a bit
		for(unsigned long int i = 0; i < llama->llama->mesh->vertices.size(); ++i){
			llama->llama->mesh->vertices.at(i).x *= sweet::NumberUtils::randomInt(50, 125)/100.f;
			llama->llama->mesh->vertices.at(i).y *= sweet::NumberUtils::randomInt(50, 125)/100.f;
		}
	}

	llama->childTransform->scale(0.1f);
	llama->childTransform->translate(glm::vec3(levelPath->vertices.at(0).x, levelPath->vertices.at(0).y, 0));

	levelPath->addLlama(llama);
}